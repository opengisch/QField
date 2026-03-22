import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtMultimedia
import org.qfield
import org.qgis
import Theme
import "../.."
import ".."

RelationEditorBase {
  id: relationEditor

  property bool isGridView: true
  property string imagePrefix: {
    if (qgisProject == undefined)
      return "";
    const path = qgisProject.homePath;
    return path.endsWith("/") ? path : path + "/";
  }

  gridView.cellWidth: isGridView ? Math.floor(gridView.width / Math.max(2, Math.floor(gridView.width / 160))) : gridView.width
  gridView.cellHeight: isGridView ? gridView.cellWidth : 72

  gridView.model: DelegateModel {
    model: referencingFeatureListModel
    delegate: galleryDelegate
  }

  headerActions: [
    QfToolButton {
      width: 48
      height: 48
      enabled: isEnabled
      visible: isEnabled

      round: false
      iconSource: Theme.getThemeVectorIcon('ic_camera_photo_black_24dp')
      iconColor: Theme.mainTextColor
      bgcolor: 'transparent'
      onClicked: {
        platformUtilities.createDir(qgisProject.homePath, 'DCIM');
        relationCameraLoader.active = true;
      }
    }
  ]

  bottomBarContent: [
    QfSwitch {
      id: viewSwitch
      anchors.right: parent.right
      anchors.rightMargin: 10
      anchors.verticalCenter: parent.verticalCenter
      width: 56 + 36
      height: 48
      checked: !isGridView
      indicator: Rectangle {
        implicitHeight: 36
        implicitWidth: 36 * 2
        x: viewSwitch.leftPadding
        radius: 4
        color: "#24212121"
        border.color: "#14FFFFFF"
        anchors.verticalCenter: parent.verticalCenter

        QfToolButton {
          width: 36
          height: 36
          anchors.left: parent.left
          anchors.verticalCenter: parent.verticalCenter
          round: false
          iconSource: Theme.getThemeVectorIcon('ic_grid_black_24dp')
          iconColor: Theme.mainTextColor
          bgcolor: 'transparent'
          enabled: false
          opacity: 0.6
        }

        QfToolButton {
          width: 36
          height: 36
          anchors.right: parent.right
          anchors.verticalCenter: parent.verticalCenter
          round: false
          iconSource: Theme.getThemeVectorIcon('ic_list_black_24dp')
          iconColor: Theme.mainTextColor
          bgcolor: 'transparent'
          enabled: false
          opacity: 0.6
        }

        Rectangle {
          x: viewSwitch.checked ? parent.width - width : 0
          width: 36
          height: 36
          radius: 4
          color: Theme.mainColor
          border.color: Theme.mainOverlayColor

          QfToolButton {
            anchors.centerIn: parent
            width: 36
            height: 36
            round: false
            iconSource: viewSwitch.checked ? Theme.getThemeVectorIcon('ic_list_black_24dp') : Theme.getThemeVectorIcon('ic_grid_black_24dp')
            iconColor: "white"
            bgcolor: 'transparent'
            enabled: false
          }

          Behavior on x {
            PropertyAnimation {
              duration: 100
              easing.type: Easing.OutQuart
            }
          }
        }
      }

      onClicked: {
        isGridView = !checked;
      }
    }
  ]

  relationEditorModel: ReferencingFeatureListModel {
    id: referencingFeatureListModel
    currentRelationId: relationId
    currentNmRelationId: nmRelationId ? nmRelationId : ""
    feature: currentFeature

    property int featureFocus: -1
    onModelUpdated: {
      if (featureFocus > -1) {
        gridView.currentIndex = referencingFeatureListModel.getFeatureIdRow(featureFocus);
        featureFocus = -1;
      }
    }
  }

  onToggleSortAction: {
    referencingFeatureListModel.sortOrder = referencingFeatureListModel.sortOrder === Qt.AscendingOrder ? Qt.DescendingOrder : Qt.AscendingOrder;
  }

  ExpressionEvaluator {
    id: attachmentNamingEvaluator
    feature: currentFeature
    layer: referencingFeatureListModel.relation ? referencingFeatureListModel.relation.referencingLayer : null
    project: qgisProject
    appExpressionContextScopesGenerator: appScopesGenerator
    expressionText: {
      var value;
      var refLayer = referencingFeatureListModel.relation ? referencingFeatureListModel.relation.referencingLayer : null;
      var fieldName = referencingFeatureListModel.attachmentFieldName;
      if (refLayer && fieldName) {
        if (refLayer.customProperty('QFieldSync/attachment_naming') !== undefined) {
          value = JSON.parse(refLayer.customProperty('QFieldSync/attachment_naming'))[fieldName];
          return value !== undefined ? value : '';
        } else if (refLayer.customProperty('QFieldSync/photo_naming') !== undefined) {
          value = JSON.parse(refLayer.customProperty('QFieldSync/photo_naming'))[fieldName];
          return value !== undefined ? value : '';
        }
      }
      return '';
    }
  }

  function getAttachmentFilePath() {
    const evaluatedFilepath = attachmentNamingEvaluator.evaluate();
    let filepath = FileUtils.sanitizeFilePath(evaluatedFilepath);
    if (FileUtils.fileSuffix(filepath) === '' && !filepath.endsWith("{extension}") && !filepath.endsWith("{filename}")) {
      const nowStr = (new Date()).toISOString().replace(/[^0-9]/g, '');
      filepath = 'DCIM/JPEG_' + nowStr + '.{extension}';
    }
    filepath = filepath.replace('\\', '/');
    return filepath;
  }

  function showAddFeaturePopup(geometry, attachmentPath) {
    ensureEmbeddedFormLoaded();
    embeddedPopup.state = 'Add';
    embeddedPopup.currentLayer = relationEditorModel.relation.referencingLayer;
    embeddedPopup.linkedParentFeature = relationEditorModel.feature;
    embeddedPopup.linkedRelation = relationEditorModel.relation;
    if (relationEditorModel.orderingField) {
      embeddedPopup.linkedRelationOrderingField = relationEditorModel.orderingField;
    }
    if (geometry !== undefined) {
      embeddedPopup.applyGeometry(geometry);
    }
    embeddedPopup.open();
    if (attachmentPath !== undefined && attachmentPath !== "") {
      const fieldName = referencingFeatureListModel.attachmentFieldName;
      embeddedPopup.attributeFormModel.applyParentDefaultValues();
      embeddedPopup.attributeFormModel.changeAttribute(fieldName, attachmentPath);
    }
  }

  function openFeatureForm(feature, nmFeature) {
    ensureEmbeddedFormLoaded();
    embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly';
    embeddedPopup.currentLayer = nmRelationId ? referencingFeatureListModel.nmRelation.referencedLayer : referencingFeatureListModel.relation.referencingLayer;
    embeddedPopup.linkedRelation = referencingFeatureListModel.relation;
    embeddedPopup.linkedParentFeature = referencingFeatureListModel.feature;
    embeddedPopup.feature = nmRelationId ? nmFeature : feature;
    embeddedPopup.open();
  }

  Loader {
    id: relationCameraLoader
    active: false
    sourceComponent: Component {
      QFieldCamera {
        Component.onCompleted: {
          state = 'PhotoCapture';
          open();
        }
        onFinished: path => {
          const filepath = StringUtils.replaceFilenameTags(getAttachmentFilePath(), path);
          platformUtilities.renameFile(path, imagePrefix + filepath);
          showAddFeaturePopup(undefined, filepath);
          close();
        }
        onCanceled: close()
        onClosed: relationCameraLoader.active = false
      }
    }
  }

  // Single adaptive delegate, switches layout based on isGridView
  Component {
    id: galleryDelegate

    Item {
      width: gridView.cellWidth
      height: gridView.cellHeight

      readonly property string attachmentFullPath: {
        const path = model.attachmentPath;
        return (path && path !== "") ? imagePrefix + path : "";
      }
      readonly property string attachmentMimeType: attachmentFullPath !== "" ? FileUtils.mimeTypeName(attachmentFullPath) : ""
      readonly property bool attachmentIsVideo: attachmentMimeType.startsWith("video/")
      readonly property bool attachmentIsImage: attachmentMimeType.startsWith("image/") && FileUtils.isImageMimeTypeSupported(attachmentMimeType)

      Loader {
        id: videoThumbLoader
        active: attachmentIsVideo
        parent: isGridView ? cardVideoContainer : listVideoContainer
        anchors.fill: parent

        property url sourceUrl: attachmentIsVideo ? UrlUtils.fromString(attachmentFullPath) : ""
        property bool firstFrameDrawn: false

        sourceComponent: Component {
          Video {
            anchors.fill: parent
            source: videoThumbLoader.sourceUrl
            muted: !cardContainer.videoPlaying || !isGridView
            volume: (isGridView && cardContainer.videoPlaying) ? 1.0 : 0
            scale: isGridView ? 2.5 : 1.5

            onHasVideoChanged: {
              if (hasVideo && !videoThumbLoader.firstFrameDrawn)
                play();
            }

            onPositionChanged: {
              if (!videoThumbLoader.firstFrameDrawn && playbackState === MediaPlayer.PlayingState) {
                videoThumbLoader.firstFrameDrawn = true;
                thumbnailPauseTimer.start();
              }
            }

            Timer {
              id: thumbnailPauseTimer
              interval: 80
              repeat: false
              onTriggered: parent.pause()
            }
          }
        }
      }

      Item {
        id: listLayout
        anchors.fill: parent
        visible: !isGridView

        Ripple {
          clip: true
          anchors.fill: parent
          pressed: listMouseArea.pressed
          anchor: listLayout
          active: listMouseArea.pressed
          color: Material.rippleColor
        }

        Row {
          id: listRow
          anchors.fill: parent
          anchors.leftMargin: 10
          anchors.rightMargin: 10
          spacing: 10

          Rectangle {
            id: listThumbnail
            width: 56
            height: 56
            anchors.verticalCenter: parent.verticalCenter
            radius: 6
            color: Theme.controlBorderColor
            clip: true

            Image {
              anchors.fill: parent
              fillMode: Image.PreserveAspectCrop
              asynchronous: true
              autoTransform: true
              visible: attachmentIsImage
              source: attachmentIsImage ? UrlUtils.fromString(attachmentFullPath) : ""
            }

            Item {
              id: listVideoContainer
              anchors.fill: parent
            }

            Image {
              anchors.centerIn: parent
              width: 28
              height: 28
              visible: !attachmentIsImage && !attachmentIsVideo
              source: Theme.getThemeVectorIcon("ic_photo_notavailable_black_24dp")
              fillMode: Image.PreserveAspectFit
              opacity: 0.3
            }

            Rectangle {
              anchors.centerIn: parent
              width: 40
              height: 40
              radius: width / 2
              color: Qt.hsla(Theme.mainBackgroundColor.hslHue, Theme.mainBackgroundColor.hslSaturation, Theme.mainBackgroundColor.hslLightness, Theme.darkTheme ? 0.75 : 0.95)
              visible: attachmentIsVideo

              QfToolButton {
                anchors.centerIn: parent
                width: 36
                height: 36
                round: false
                iconSource: Theme.getThemeVectorIcon("ic_play_black_24dp")
                iconColor: "white"
                enabled: false
              }
            }
          }

          Text {
            anchors.verticalCenter: parent.verticalCenter
            width: listRow.width - listThumbnail.width - listMenuButton.width - listRow.spacing * 2
            text: model.displayString
            color: Theme.mainTextColor
            font: Theme.defaultFont
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
            maximumLineCount: 2
          }

          QfToolButton {
            id: listMenuButton
            anchors.verticalCenter: parent.verticalCenter
            width: 48
            height: 48
            round: false
            iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'

            onClicked: {
              childMenu.entryReferencingFeature = model.referencingFeature;
              childMenu.entryDisplayString = model.displayString;
              childMenu.entryNmReferencedFeature = nmRelationId ? model.nmReferencedFeature : undefined;
              childMenu.entryNmReferencedFeatureDisplayMessage = nmRelationId ? model.nmDisplayString : '';
              const pos = listMenuButton.mapToItem(relationEditor, 0, 0);
              childMenu.popup(pos.x, pos.y);
            }
          }
        }

        MouseArea {
          id: listMouseArea
          anchors.fill: parent
          anchors.rightMargin: listMenuButton.width
          onClicked: openFeatureForm(model.referencingFeature, model.nmReferencedFeature)
        }
      }

      Rectangle {
        id: cardContainer
        anchors.fill: parent
        anchors.margins: 4
        radius: 10
        color: Theme.controlBorderColor
        clip: true
        visible: isGridView

        property bool videoPlaying: false

        // Shared semi-opaque overlay color used by detailsBar and play button background
        readonly property color overlayColor: Qt.hsla(Theme.mainBackgroundColor.hslHue, Theme.mainBackgroundColor.hslSaturation, Theme.mainBackgroundColor.hslLightness, Theme.darkTheme ? 0.75 : 0.95)

        Rectangle {
          id: roundMask
          anchors.fill: parent
          radius: parent.radius
          color: "white"
          visible: false
          layer.enabled: true
        }

        Image {
          id: cardThumb
          anchors.fill: parent
          fillMode: Image.PreserveAspectCrop
          asynchronous: true
          autoTransform: true
          cache: true
          visible: !attachmentIsVideo
          source: attachmentIsVideo ? "" : UrlUtils.fromString(attachmentFullPath)

          layer.enabled: true
          layer.effect: QfOpacityMask {
            maskSource: roundMask
          }
        }

        Item {
          id: cardVideoContainer
          anchors.fill: parent

          layer.enabled: true
          layer.effect: QfOpacityMask {
            maskSource: roundMask
          }
        }

        Image {
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.verticalCenter: parent.verticalCenter
          anchors.verticalCenterOffset: -(detailsBar.height / 2)
          width: 44
          height: 44
          visible: cardThumb.status !== Image.Ready && !attachmentIsVideo
          source: Theme.getThemeVectorIcon("ic_photo_notavailable_black_24dp")
          fillMode: Image.PreserveAspectFit
          opacity: 0.3
        }

        Rectangle {
          anchors.centerIn: parent
          anchors.verticalCenterOffset: -(detailsBar.height / 2)
          width: 52
          height: 52
          radius: width / 2
          color: cardContainer.overlayColor
          visible: attachmentIsVideo && !cardContainer.videoPlaying

          QfToolButton {
            anchors.centerIn: parent
            width: 44
            height: 44
            round: false
            iconSource: Theme.getThemeVectorIcon("ic_play_black_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'
            enabled: false
          }
        }

        Ripple {
          clip: true
          anchors.fill: parent
          pressed: cardMouseArea.pressed
          anchor: cardContainer
          active: cardMouseArea.pressed
          color: Material.rippleColor
        }

        Rectangle {
          id: detailsBar
          anchors.bottom: parent.bottom
          anchors.left: parent.left
          anchors.right: parent.right
          height: 44
          color: cardContainer.overlayColor

          Text {
            anchors {
              left: parent.left
              leftMargin: 10
              right: cardMenuButton.left
              rightMargin: 4
              verticalCenter: parent.verticalCenter
            }
            text: model.displayString
            color: Theme.mainTextColor
            font.pixelSize: Theme.tipFont.pixelSize
            font.weight: Font.Medium
            elide: Text.ElideRight
          }

          QfToolButton {
            id: cardMenuButton
            anchors.right: parent.right
            anchors.rightMargin: 2
            anchors.verticalCenter: parent.verticalCenter
            width: 48
            height: 48

            round: false
            iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'

            onClicked: {
              childMenu.entryReferencingFeature = model.referencingFeature;
              childMenu.entryDisplayString = model.displayString;
              childMenu.entryNmReferencedFeature = nmRelationId ? model.nmReferencedFeature : undefined;
              childMenu.entryNmReferencedFeatureDisplayMessage = nmRelationId ? model.nmDisplayString : '';
              const pos = cardMenuButton.mapToItem(relationEditor, 0, 0);
              childMenu.popup(pos.x, pos.y);
            }
          }

          MouseArea {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: cardMenuButton.left
            onClicked: openFeatureForm(model.referencingFeature, model.nmReferencedFeature)
          }
        }

        MouseArea {
          id: cardMouseArea
          anchors.top: parent.top
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.bottom: detailsBar.top
          onClicked: {
            if (attachmentIsVideo && videoThumbLoader.item) {
              cardContainer.videoPlaying = !cardContainer.videoPlaying;
              if (cardContainer.videoPlaying)
                videoThumbLoader.item.play();
              else
                videoThumbLoader.item.pause();
            } else {
              openFeatureForm(model.referencingFeature, model.nmReferencedFeature);
            }
          }
        }
      }
    }
  }
}
