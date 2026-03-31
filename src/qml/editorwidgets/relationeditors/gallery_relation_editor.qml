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
import "../ExternalResourceUtils.js" as ExternalResourceUtils

RelationEditorBase {
  id: relationEditor

  property bool isGridView: true
  property string imagePrefix: {
    if (qgisProject == undefined)
      return "";
    const path = qgisProject.homePath;
    return path.endsWith("/") ? path : path + "/";
  }

  property int documentViewer: referencingFeatureListModel.attachmentDocumentViewer

  property var activeMediaItem: null

  function requestMediaFocus(item) {
    if (activeMediaItem && activeMediaItem !== item) {
      if (typeof activeMediaItem.stopPlayback === "function") {
        activeMediaItem.stopPlayback();
      }
    }
    activeMediaItem = item;
  }

  function releaseMediaFocus(item) {
    if (activeMediaItem === item) {
      activeMediaItem = null;
    }
  }

  function stopAllMedia() {
    if (activeMediaItem) {
      if (typeof activeMediaItem.stopPlayback === "function") {
        activeMediaItem.stopPlayback();
      }
    }
    activeMediaItem = null;
  }

  onVisibleChanged: {
    if (!visible) {
      stopAllMedia();
    }
  }
  Component.onDestruction: {
    stopAllMedia();
    relationCameraLoader.active = false;
    relationAudioRecorderLoader.active = false;
  }

  property ResourceSource resourceSource
  Connections {
    target: resourceSource
    function onResourceReceived(path) {
      if (path) {
        if (documentViewer === ExternalResource.DocumentImage) {
          let maximumWidthHeight = iface.readProjectNumEntry("qfieldsync", "maximumImageWidthHeight", 0);
          if (maximumWidthHeight > 0) {
            FileUtils.restrictImageSize(imagePrefix + path, maximumWidthHeight);
          }
        }
        showAddFeaturePopup(undefined, path);
      }
    }
  }

  gridView.cellWidth: isGridView ? Math.floor(gridView.width / Math.max(2, Math.floor(gridView.width / 160))) : gridView.width
  gridView.cellHeight: isGridView ? gridView.cellWidth : 72

  gridView.model: DelegateModel {
    model: referencingFeatureListModel
    delegate: isGridView ? gridDelegate : listDelegate
  }

  onIsGridViewChanged: stopAllMedia()

  ExpressionEvaluator {
    id: attachmentNamingEvaluator
    feature: currentFeature
    layer: referencingFeatureListModel.relation ? referencingFeatureListModel.relation.referencingLayer : null
    project: qgisProject
    appExpressionContextScopesGenerator: appScopesGenerator
    expressionText: {
      let refLayer = referencingFeatureListModel.relation ? referencingFeatureListModel.relation.referencingLayer : null;
      let fieldName = referencingFeatureListModel.attachmentFieldName;
      return ExternalResourceUtils.getAttachmentNaming(refLayer, fieldName);
    }
  }

  function capturePhoto() {
    stopAllMedia();
    platformUtilities.createDir(qgisProject.homePath, 'DCIM');
    if (platformUtilities.capabilities & PlatformUtilities.NativeCamera && settings.valueBool("nativeCamera2", true)) {
      let filepath = ExternalResourceUtils.getAttachmentFilePath(attachmentNamingEvaluator.evaluate(), documentViewer, FileUtils);
      filepath = filepath.replace('{extension}', 'JPG');
      resourceSource = platformUtilities.getCameraPicture(imagePrefix, filepath, FileUtils.fileSuffix(filepath), relationEditor);
    } else {
      relationCameraLoader.isVideo = false;
      relationCameraLoader.active = true;
    }
  }

  function captureVideo() {
    stopAllMedia();
    platformUtilities.createDir(qgisProject.homePath, 'DCIM');
    if (platformUtilities.capabilities & PlatformUtilities.NativeCamera && settings.valueBool("nativeCamera2", true)) {
      let filepath = ExternalResourceUtils.getAttachmentFilePath(attachmentNamingEvaluator.evaluate(), documentViewer, FileUtils);
      filepath = filepath.replace('{extension}', 'MP4');
      resourceSource = platformUtilities.getCameraVideo(imagePrefix, filepath, FileUtils.fileSuffix(filepath), relationEditor);
    } else {
      relationCameraLoader.isVideo = true;
      relationCameraLoader.active = true;
    }
  }

  function captureAudio() {
    stopAllMedia();
    Qt.inputMethod.hide();
    relationAudioRecorderLoader.active = true;
  }

  headerActions: [
    QfToolButton {
      width: 48
      height: 48
      enabled: isEnabled
      visible: isEnabled

      round: false
      iconSource: {
        switch (documentViewer) {
        case ExternalResource.DocumentVideo:
          return Theme.getThemeVectorIcon("ic_camera_video_black_24dp");
        case ExternalResource.DocumentAudio:
          return Theme.getThemeVectorIcon("ic_microphone_black_24dp");
        default:
          return Theme.getThemeVectorIcon("ic_camera_photo_black_24dp");
        }
      }
      iconColor: Theme.mainTextColor
      bgcolor: 'transparent'
      onClicked: {
        switch (documentViewer) {
        case ExternalResource.DocumentVideo:
          captureVideo();
          break;
        case ExternalResource.DocumentAudio:
          captureAudio();
          break;
        default:
          capturePhoto();
          break;
        }
      }
    }
  ]

  footerContent: [
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
    stopAllMedia();
    referencingFeatureListModel.sortOrder = referencingFeatureListModel.sortOrder === Qt.AscendingOrder ? Qt.DescendingOrder : Qt.AscendingOrder;
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
    embeddedPopup.attributeFormModel.applyParentDefaultValues();
    if (attachmentPath !== undefined && attachmentPath !== "") {
      const fieldName = referencingFeatureListModel.attachmentFieldName;
      embeddedPopup.attributeFormModel.changeAttribute(fieldName, attachmentPath);
    }
  }

  function openFeatureForm(feature, nmFeature) {
    stopAllMedia();
    ensureEmbeddedFormLoaded();
    embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly';
    embeddedPopup.currentLayer = nmRelationId ? referencingFeatureListModel.nmRelation.referencedLayer : referencingFeatureListModel.relation.referencingLayer;
    embeddedPopup.linkedRelation = referencingFeatureListModel.relation;
    embeddedPopup.linkedParentFeature = referencingFeatureListModel.feature;
    embeddedPopup.feature = nmRelationId ? nmFeature : feature;
    embeddedPopup.open();
  }

  function resolveAttachmentPath(path) {
    if (!path || path === "") {
      return "";
    }
    if (path.startsWith("http://") || path.startsWith("https://")) {
      return path;
    }
    if (FileUtils.fileExists(path)) {
      return path;
    }
    return imagePrefix + path;
  }

  Loader {
    id: relationCameraLoader
    active: false
    property bool isVideo: false
    sourceComponent: Component {
      QFieldCamera {
        Component.onCompleted: {
          state = relationCameraLoader.isVideo ? 'VideoCapture' : 'PhotoCapture';
          open();
        }
        onFinished: path => {
          const filepath = StringUtils.replaceFilenameTags(ExternalResourceUtils.getAttachmentFilePath(attachmentNamingEvaluator.evaluate(), documentViewer, FileUtils), path);
          platformUtilities.renameFile(path, imagePrefix + filepath);
          if (!relationCameraLoader.isVideo) {
            let maximumWidthHeight = iface.readProjectNumEntry("qfieldsync", "maximumImageWidthHeight", 0);
            if (maximumWidthHeight > 0) {
              FileUtils.restrictImageSize(imagePrefix + filepath, maximumWidthHeight);
            }
          }
          showAddFeaturePopup(undefined, filepath);
          close();
        }
        onCanceled: close()
        onClosed: relationCameraLoader.active = false
      }
    }
  }

  Loader {
    id: relationAudioRecorderLoader
    active: false
    sourceComponent: Component {
      QFieldAudioRecorder {
        z: 10000
        visible: false
        Component.onCompleted: open()
        onFinished: path => {
          const filepath = StringUtils.replaceFilenameTags(ExternalResourceUtils.getAttachmentFilePath(attachmentNamingEvaluator.evaluate(), documentViewer, FileUtils), path);
          platformUtilities.renameFile(path, imagePrefix + filepath);
          showAddFeaturePopup(undefined, filepath);
          close();
        }
        onCanceled: close()
        onClosed: relationAudioRecorderLoader.active = false
      }
    }
  }

  Component {
    id: listDelegate

    Item {
      width: gridView.cellWidth
      height: gridView.cellHeight

      readonly property string attachmentFullPath: resolveAttachmentPath(model.attachmentPath)
      readonly property string attachmentMimeType: attachmentFullPath !== "" ? FileUtils.mimeTypeName(attachmentFullPath) : ""
      readonly property bool attachmentIsVideo: attachmentMimeType.startsWith("video/")
      readonly property bool attachmentIsAudio: attachmentMimeType.startsWith("audio/")
      readonly property bool attachmentIsImage: attachmentMimeType.startsWith("image/") && FileUtils.isImageMimeTypeSupported(attachmentMimeType)

      Loader {
        id: listVideoThumbLoader
        active: attachmentIsVideo
        parent: listVideoContainer
        anchors.fill: parent

        property url sourceUrl: attachmentIsVideo ? UrlUtils.fromString(attachmentFullPath) : ""
        property bool firstFrameDrawn: false

        sourceComponent: Component {
          Video {
            anchors.fill: parent
            source: listVideoThumbLoader.sourceUrl
            muted: true
            volume: 0
            scale: 1.5

            onHasVideoChanged: {
              if (hasVideo && !listVideoThumbLoader.firstFrameDrawn) {
                play();
              }
            }

            onPositionChanged: {
              if (!listVideoThumbLoader.firstFrameDrawn && playbackState === MediaPlayer.PlayingState) {
                listVideoThumbLoader.firstFrameDrawn = true;
                listThumbnailPauseTimer.start();
              }
            }

            Component.onDestruction: stop()

            Timer {
              id: listThumbnailPauseTimer
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

            Rectangle {
              id: listRoundMask
              anchors.fill: parent
              radius: parent.radius
              color: "white"
              visible: false
              layer.enabled: true
            }

            Image {
              anchors.fill: parent
              fillMode: Image.PreserveAspectCrop
              asynchronous: true
              autoTransform: true
              visible: attachmentIsImage
              source: attachmentIsImage ? UrlUtils.fromString(attachmentFullPath) : ""

              layer.enabled: true
              layer.effect: QfOpacityMask {
                maskSource: listRoundMask
              }
            }

            Item {
              id: listVideoContainer
              anchors.fill: parent
            }

            // Hash-seeded waveform bars (decorative, each file gets a unique pattern)
            Row {
              anchors.centerIn: parent
              spacing: 1
              visible: attachmentIsAudio

              Repeater {
                model: 18

                Rectangle {
                  width: 2
                  height: {
                    const hash = ExternalResourceUtils.hashString(attachmentFullPath);
                    const seed = (hash + index) * 0.3;
                    const h = 8 + Math.abs(Math.sin(seed)) * 28 + Math.abs(Math.cos(seed * 2.1)) * 12;
                    return Math.min(h, 44);
                  }
                  radius: 1
                  anchors.verticalCenter: parent.verticalCenter
                  color: Theme.mainColor
                  opacity: 0.4
                }
              }
            }

            Image {
              anchors.centerIn: parent
              width: 28
              height: 28
              visible: !attachmentIsImage && !attachmentIsVideo && !attachmentIsAudio
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
                iconColor: Theme.mainTextColor
                bgcolor: 'transparent'
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
    }
  }

  Component {
    id: gridDelegate

    Item {
      width: gridView.cellWidth
      height: gridView.cellHeight

      readonly property string attachmentFullPath: resolveAttachmentPath(model.attachmentPath)
      readonly property string attachmentMimeType: attachmentFullPath !== "" ? FileUtils.mimeTypeName(attachmentFullPath) : ""
      readonly property bool attachmentIsVideo: attachmentMimeType.startsWith("video/")
      readonly property bool attachmentIsAudio: attachmentMimeType.startsWith("audio/")
      readonly property bool attachmentIsImage: attachmentMimeType.startsWith("image/") && FileUtils.isImageMimeTypeSupported(attachmentMimeType)

      Component.onDestruction: {
        if (cardContainer.videoPlaying && videoThumbLoader.item) {
          cardContainer.stopPlayback();
        }
        if (audioPlayerLoader.active && audioPlayerLoader.isPlaying && audioPlayerLoader.item) {
          audioPlayerLoader.item.togglePlayback();
        }
        relationEditor.releaseMediaFocus(cardContainer);
        relationEditor.releaseMediaFocus(audioPlayerLoader);
      }

      Loader {
        id: videoThumbLoader
        active: attachmentIsVideo
        parent: cardVideoContainer
        anchors.fill: parent

        property url sourceUrl: attachmentIsVideo ? UrlUtils.fromString(attachmentFullPath) : ""
        property bool firstFrameDrawn: false

        sourceComponent: Component {
          Video {
            anchors.fill: parent
            source: videoThumbLoader.sourceUrl
            muted: !cardContainer.videoPlaying
            volume: cardContainer.videoPlaying ? 1.0 : 0
            scale: 2.5

            onHasVideoChanged: {
              if (hasVideo && !videoThumbLoader.firstFrameDrawn) {
                play();
              }
            }

            onPositionChanged: {
              if (!videoThumbLoader.firstFrameDrawn && playbackState === MediaPlayer.PlayingState) {
                videoThumbLoader.firstFrameDrawn = true;
                thumbnailPauseTimer.start();
              }
            }

            Component.onDestruction: stop()

            Timer {
              id: thumbnailPauseTimer
              interval: 80
              repeat: false
              onTriggered: parent.pause()
            }
          }
        }
      }

      Loader {
        id: audioPlayerLoader
        active: attachmentIsAudio

        property url sourceUrl: attachmentIsAudio ? UrlUtils.fromString(attachmentFullPath) : ""
        property bool isPlaying: false
        property real progress: 0

        function stopPlayback() {
          if (item && isPlaying) {
            item.togglePlayback();
          }
          isPlaying = false;
        }

        sourceComponent: Component {
          Item {
            MediaPlayer {
              id: audioPlayer
              source: audioPlayerLoader.sourceUrl

              audioOutput: AudioOutput {
                volume: 1.0
              }
            }

            Connections {
              target: audioPlayer
              function onPositionChanged() {
                if (audioPlayer.duration > 0) {
                  audioPlayerLoader.progress = audioPlayer.position / audioPlayer.duration;
                }
              }
              function onPlaybackStateChanged() {
                audioPlayerLoader.isPlaying = (audioPlayer.playbackState === MediaPlayer.PlayingState);
                if (audioPlayer.playbackState !== MediaPlayer.PlayingState) {
                  relationEditor.releaseMediaFocus(audioPlayerLoader);
                }
              }
            }

            function togglePlayback() {
              if (audioPlayer.playbackState === MediaPlayer.PlayingState) {
                audioPlayer.pause();
              } else {
                relationEditor.requestMediaFocus(audioPlayerLoader);
                audioPlayer.play();
              }
            }

            Component.onDestruction: audioPlayer.stop()

            property alias duration: audioPlayer.duration
            property alias position: audioPlayer.position
            property alias playbackState: audioPlayer.playbackState
          }
        }
      }

      Rectangle {
        id: cardContainer
        anchors.fill: parent
        anchors.margins: 4
        radius: 10
        color: Theme.controlBorderColor
        clip: true
        visible: true

        property bool videoPlaying: false

        function stopPlayback() {
          if (videoPlaying && videoThumbLoader.item) {
            videoThumbLoader.item.pause();
          }
          videoPlaying = false;
          relationEditor.releaseMediaFocus(cardContainer);
        }

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
          id: cardThumbnail
          anchors.fill: parent
          fillMode: Image.PreserveAspectCrop
          asynchronous: true
          autoTransform: true
          cache: true
          visible: !attachmentIsVideo && !attachmentIsAudio
          source: (attachmentIsVideo || attachmentIsAudio) ? "" : UrlUtils.fromString(attachmentFullPath)

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

        Item {
          id: audioWaveformArea
          anchors.fill: parent
          anchors.bottomMargin: detailsBar.height
          visible: attachmentIsAudio
          clip: true

          Row {
            id: waveformBars
            anchors.centerIn: parent
            height: parent.height * 0.7
            spacing: 2

            Repeater {
              model: Math.max(1, Math.floor((audioWaveformArea.width - 24) / 5))

              Rectangle {
                width: 3
                height: {
                  const hash = ExternalResourceUtils.hashString(attachmentFullPath);
                  const seed = (hash + index) * 0.3;
                  const h = 0.15 + Math.abs(Math.sin(seed)) * 0.55 + Math.abs(Math.cos(seed * 2.1)) * 0.3;
                  return Math.max(4, waveformBars.height * h);
                }
                radius: 1.5
                anchors.verticalCenter: parent.verticalCenter
                color: {
                  const totalBars = Math.max(1, Math.floor((audioWaveformArea.width - 24) / 5));
                  if (audioPlayerLoader.active && (index / totalBars) < audioPlayerLoader.progress) {
                    return Theme.mainColor;
                  }
                  return Theme.mainTextDisabledColor;
                }
                opacity: {
                  const totalBars = Math.max(1, Math.floor((audioWaveformArea.width - 24) / 5));
                  if (audioPlayerLoader.active && (index / totalBars) < audioPlayerLoader.progress) {
                    return 0.9;
                  }
                  return 0.35;
                }
              }
            }
          }

          Rectangle {
            visible: audioPlayerLoader.active && audioPlayerLoader.progress > 0
            x: waveformBars.x + waveformBars.width * audioPlayerLoader.progress
            y: 0
            width: 2
            height: parent.height
            radius: 1
            color: Theme.mainColor
          }

          Rectangle {
            anchors.centerIn: parent
            width: 48
            height: 48
            radius: width / 2
            color: cardContainer.overlayColor
            border.width: 0.5
            border.color: Theme.controlBorderColor

            QfToolButton {
              anchors.centerIn: parent
              width: 40
              height: 40
              round: false
              iconSource: audioPlayerLoader.isPlaying ? Theme.getThemeVectorIcon("ic_pause_black_24dp") : Theme.getThemeVectorIcon("ic_play_black_24dp")
              iconColor: Theme.mainTextColor
              bgcolor: 'transparent'
              enabled: false
            }
          }
        }

        Image {
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.verticalCenter: parent.verticalCenter
          anchors.verticalCenterOffset: -(detailsBar.height / 2)
          width: 44
          height: 44
          visible: cardThumbnail.status !== Image.Ready && !attachmentIsVideo && !attachmentIsAudio
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
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: cardMenuButton.left
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter

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
              if (cardContainer.videoPlaying) {
                cardContainer.stopPlayback();
              } else {
                relationEditor.requestMediaFocus(cardContainer);
                cardContainer.videoPlaying = true;
                videoThumbLoader.item.play();
              }
            } else if (attachmentIsAudio && audioPlayerLoader.item) {
              audioPlayerLoader.item.togglePlayback();
            } else {
              openFeatureForm(model.referencingFeature, model.nmReferencedFeature);
            }
          }
        }
      }
    }
  }
}
