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

  property string pendingAttachmentPath: ""
  property string imagePrefix: {
    if (qgisProject == undefined)
      return "";
    let path = qgisProject.homePath;
    return path.endsWith("/") ? path : path + "/";
  }

  showCameraButton: true
  listView.visible: false
  gridView.visible: true
  gridView.model: DelegateModel {
    model: referencingFeatureListModel
    delegate: galleryDelegate
  }

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

  onCameraAction: {
    platformUtilities.createDir(qgisProject.homePath, 'DCIM');
    relationCameraLoader.active = true;
  }

  Connections {
    target: embeddedPopup
    function onOpened() {
      if (pendingAttachmentPath !== "") {
        const fieldName = referencingFeatureListModel.attachmentFieldName;
        const relId = referencingFeatureListModel.currentRelationId;
        console.log("fieldName:", fieldName, "relationId:", relId);
        console.log("layer name:", relationEditorModel.relation.referencingLayer ? relationEditorModel.relation.referencingLayer.name : "NULL")
        embeddedPopup.attributeFormModel.applyParentDefaultValues();
        embeddedPopup.attributeFormModel.changeAttribute(fieldName, pendingAttachmentPath);
        pendingAttachmentPath = "";
      }
    }
  }

  function showAddFeaturePopup(geometry) {
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
    embeddedPopup.attributeFormModel.applyParentDefaultValues();
    if (pendingAttachmentPath !== "") {
      embeddedPopup.attributeFormModel.changeAttribute(attachmentFieldName, pendingAttachmentPath);
      pendingAttachmentPath = "";
    }
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
          const nowStr = (new Date()).toISOString().replace(/[^0-9]/g, '');
          const ext = FileUtils.fileSuffix(path).toLowerCase();
          const filepath = 'DCIM/JPEG_' + nowStr + '.' + ext;
          platformUtilities.renameFile(path, imagePrefix + filepath);
          pendingAttachmentPath = filepath;
          showAddFeaturePopup();
          close();
        }
        onCanceled: close()
        onClosed: relationCameraLoader.active = false
      }
    }
  }

  Component {
    id: galleryDelegate

    Item {
      width: gridView.cellWidth
      height: gridView.cellHeight

      Rectangle {
        id: cardContainer
        anchors.fill: parent
        anchors.margins: 4
        radius: 10
        color: Theme.controlBorderColor
        clip: true

        readonly property string attachmentFullPath: {
          var path = model.attachmentPath;
          return (path && path !== "") ? imagePrefix + path : "";
        }
        readonly property string attachmentMimeType: attachmentFullPath !== "" ? FileUtils.mimeTypeName(attachmentFullPath) : ""
        readonly property bool attachmentIsVideo: attachmentMimeType.startsWith("video/")

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
          visible: !cardContainer.attachmentIsVideo

          source: {
            var path = model.attachmentPath;
            if (!path || path === "")
              return "";
            return UrlUtils.fromString(imagePrefix + path);
          }

          layer.enabled: true
          layer.effect: QfOpacityMask {
            maskSource: roundMask
          }
        }

        Loader {
          id: videoThumbLoader
          anchors.fill: parent
          active: cardContainer.attachmentIsVideo

          property url sourceUrl: cardContainer.attachmentIsVideo ? UrlUtils.fromString(cardContainer.attachmentFullPath) : ""
          property bool firstFrameDrawn: false

          sourceComponent: Component {
            Video {
              anchors.fill: parent
              source: videoThumbLoader.sourceUrl
              muted: true
              volume: 0

              onHasVideoChanged: {
                if (hasVideo && !videoThumbLoader.firstFrameDrawn)
                  play();
              }

              onPositionChanged: {
                if (!videoThumbLoader.firstFrameDrawn && playbackState === MediaPlayer.PlayingState) {
                  videoThumbLoader.firstFrameDrawn = true;
                  pause();
                }
              }
            }
          }
        }

        Image {
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.verticalCenter: parent.verticalCenter
          anchors.verticalCenterOffset: -(detailsBar.height / 2)
          width: 44
          height: 44
          visible: cardThumb.status !== Image.Ready && !cardContainer.attachmentIsVideo
          source: Theme.getThemeVectorIcon("ic_photo_notavailable_black_24dp")
          fillMode: Image.PreserveAspectFit
          opacity: 0.3
        }

        QfToolButton {
          anchors.centerIn: parent
          anchors.verticalCenterOffset: -(detailsBar.height / 2)
          width: 46
          height: 46
          round: false
          iconSource: Theme.getThemeVectorIcon("ic_play_black_24dp")
          iconColor: "white"
          bgcolor: 'transparent'
          enabled: false
          visible: cardContainer.attachmentIsVideo
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
          color: Qt.hsla(Theme.mainBackgroundColor.hslHue, Theme.mainBackgroundColor.hslSaturation, Theme.mainBackgroundColor.hslLightness, Theme.darkTheme ? 0.75 : 0.95)

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
        }

        MouseArea {
          id: cardMouseArea
          anchors.top: parent.top
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.bottom: detailsBar.top
          onClicked: {
            ensureEmbeddedFormLoaded();
            embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly';
            embeddedPopup.currentLayer = nmRelationId ? referencingFeatureListModel.nmRelation.referencedLayer : referencingFeatureListModel.relation.referencingLayer;
            embeddedPopup.linkedRelation = referencingFeatureListModel.relation;
            embeddedPopup.linkedParentFeature = referencingFeatureListModel.feature;
            embeddedPopup.feature = nmRelationId ? model.nmReferencedFeature : model.referencingFeature;
            embeddedPopup.open();
          }
        }
      }
    }
  }
}
