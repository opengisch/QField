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

  property var pendingDownloads: ({})
  property var failedDownloads: ({})
  property var fetchedPaths: ({})
  property int downloadRevision: 0

  // ExternalStorage handles one fetch at a time, so requests are queued and dispatched sequentially as each one completes
  property var fetchQueue: []
  property string currentFetchKey: ""

  function enqueueExternalFetch(relativePath, fetchUrl, authConfigId) {
    fetchQueue.push({
      key: relativePath,
      url: fetchUrl,
      authId: authConfigId
    });
    processNextFetch();
  }

  function processNextFetch() {
    if (currentFetchKey !== "" || fetchQueue.length === 0)
      return;
    const next = fetchQueue.shift();
    currentFetchKey = next.key;
    externalStorage.fetch(next.url, next.authId);
  }

  property var videoQueue: []
  property bool videoQueueBusy: false

  Timer {
    id: videoQueueTimer
    interval: 150
    onTriggered: {
      while (videoQueue.length > 0) {
        if (videoQueue.shift()()) {
          videoQueueTimer.start();
          return;
        }
      }
      videoQueueBusy = false;
    }
  }

  function queueVideoLoad(fn) {
    videoQueue.push(fn);
    if (!videoQueueBusy) {
      videoQueueBusy = true;
      videoQueueTimer.start();
    }
  }

  QtObject {
    id: dummyTarget
  }

  Connections {
    id: cloudProjectConnection
    ignoreUnknownSignals: true
    target: dummyTarget

    function onDownloadAttachmentFinished(fileName, errorString) {
      if (relationEditor.pendingDownloads.hasOwnProperty(fileName)) {
        delete relationEditor.pendingDownloads[fileName];
        if (errorString !== "") {
          relationEditor.failedDownloads[fileName] = true;
          displayToast(qsTr("QFieldCloud on-demand attachment error: ") + errorString, 'error');
        }
        if (Object.keys(relationEditor.pendingDownloads).length === 0) {
          cloudProjectConnection.target = dummyTarget;
        }
        relationEditor.downloadRevision++;
      }
    }
  }

  ExternalStorage {
    id: externalStorage
    type: referencingFeatureListModel.attachmentStorageType

    onFetchedContentChanged: {
      if (fetchedContent === "" || relationEditor.currentFetchKey === "")
        return;
      relationEditor.fetchedPaths[relationEditor.currentFetchKey] = fetchedContent;
      delete relationEditor.pendingDownloads[relationEditor.currentFetchKey];
      relationEditor.currentFetchKey = "";
      relationEditor.downloadRevision++;
      relationEditor.processNextFetch();
    }

    onLastErrorChanged: {
      if (relationEditor.currentFetchKey !== "") {
        relationEditor.failedDownloads[relationEditor.currentFetchKey] = true;
        delete relationEditor.pendingDownloads[relationEditor.currentFetchKey];
        relationEditor.currentFetchKey = "";
        relationEditor.downloadRevision++;
        relationEditor.processNextFetch();
      }
      displayToast(lastError, 'error');
    }
  }

  AudioAnalyzer {
    id: audioAnalyzer

    property var queue: []
    property string currentProcess: ""
    property var availableBars: ({})

    barCount: isGridView ? 40 : 20

    onBarCountChanged: {
      availableBars = {};
    }

    onReady: bars => {
      availableBars[currentProcess] = bars;
      availableBarsChanged();
      processQueue();
    }

    function enqueue(audioPath) {
      if (!(audioPath in queue) && !(audioPath in availableBars)) {
        queue.push(audioPath);
        processQueue();
      }
    }

    function processQueue() {
      if (queue.length > 0) {
        currentProcess = queue.shift();
        audioAnalyzer.analyze(UrlUtils.fromString(currentProcess));
      }
    }
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

  readonly property int visibleCards: Math.max(2, Math.floor(gridView.width / 180))
  readonly property int cardSize: Math.floor(gridView.width / (visibleCards + 0.5))
  readonly property int listItemHeight: 72
  readonly property int toggleHeight: 40
  readonly property int gridMargin: 8

  bottomMargin: 10 + (itemCount > 0 ? toggleHeight : 0)

  height: {
    const toggleSpace = itemCount > 0 ? toggleHeight : 0;
    if (isGridView) {
      if (itemCount === 0) {
        return headerEntry.height + 10 + toggleSpace;
      }
      return cardSize + gridMargin * 2 + headerEntry.height + 10 + toggleSpace;
    }
    const cappedHeight = !showAllItems && maximumVisibleItems > 0 ? Math.min(maximumVisibleItems * gridView.cellHeight, gridView.contentHeight) : gridView.contentHeight;
    return cappedHeight + headerEntry.height + 10 + toggleSpace;
  }

  gridView.flow: isGridView ? GridView.FlowTopToBottom : GridView.FlowLeftToRight
  gridView.flickableDirection: isGridView ? Flickable.HorizontalFlick : Flickable.VerticalFlick
  gridView.topMargin: isGridView ? gridMargin : 0
  gridView.bottomMargin: isGridView ? gridMargin : 0
  gridView.ScrollBar.vertical.policy: isGridView ? ScrollBar.AlwaysOff : ScrollBar.AsNeeded

  gridView.cellWidth: isGridView ? cardSize : gridView.width
  gridView.cellHeight: isGridView ? cardSize : listItemHeight

  gridView.model: DelegateModel {
    model: referencingFeatureListModel
    delegate: isGridView ? gridDelegate : listDelegate
  }

  onIsGridViewChanged: stopAllMedia()

  ExpressionEvaluator {
    id: attachmentNamingEvaluator
    layer: referencingFeatureListModel.relation ? referencingFeatureListModel.relation.referencingLayer : null
    project: qgisProject
    appExpressionContextScopesGenerator: appScopesGenerator
  }

  function capturePhoto() {
    stopAllMedia();
    Qt.inputMethod.hide();
    prepareFeature();
    platformUtilities.createDir(qgisProject.homePath, 'DCIM');
    attachmentNamingEvaluator.expressionText = ExternalResourceUtils.getAttachmentNaming(referencingFeatureListModel.relation ? referencingFeatureListModel.relation.referencingLayer : null, referencingFeatureListModel.attachmentFieldName);
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
    Qt.inputMethod.hide();
    prepareFeature();
    platformUtilities.createDir(qgisProject.homePath, 'DCIM');
    attachmentNamingEvaluator.expressionText = ExternalResourceUtils.getAttachmentNaming(referencingFeatureListModel.relation ? referencingFeatureListModel.relation.referencingLayer : null, referencingFeatureListModel.attachmentFieldName);
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
    prepareFeature();
    attachmentNamingEvaluator.expressionText = ExternalResourceUtils.getAttachmentNaming(referencingFeatureListModel.relation ? referencingFeatureListModel.relation.referencingLayer : null, referencingFeatureListModel.attachmentFieldName);
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
        if (!prepareParent()) {
          return;
        }

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

  QfSwitch {
    id: viewSwitch

    readonly property int slotSize: 36
    readonly property int highlightInset: 1

    anchors.right: parent.right
    anchors.rightMargin: 1
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 2
    visible: itemCount > 0
    padding: 0
    width: slotSize * 2 + 4
    height: toggleHeight
    checked: !isGridView
    indicator: Rectangle {
      implicitHeight: viewSwitch.slotSize
      implicitWidth: viewSwitch.slotSize * 2
      x: (viewSwitch.width - implicitWidth) / 2
      radius: 4
      color: "transparent"
      anchors.verticalCenter: parent.verticalCenter

      QfToolButton {
        width: viewSwitch.slotSize
        height: viewSwitch.slotSize
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        round: false
        iconSource: Theme.getThemeVectorIcon('ic_grid_black_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: 'transparent'
        enabled: false
        opacity: 0.3
      }

      QfToolButton {
        width: viewSwitch.slotSize
        height: viewSwitch.slotSize
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        round: false
        iconSource: Theme.getThemeVectorIcon('ic_list_black_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: 'transparent'
        enabled: false
        opacity: 0.3
      }

      Rectangle {
        readonly property int inset: viewSwitch.highlightInset
        x: viewSwitch.checked ? parent.width - width - inset : inset
        y: inset
        width: viewSwitch.slotSize - inset * 2
        height: viewSwitch.slotSize - inset * 2
        radius: 3
        color: Theme.mainBackgroundColor
        border.color: Theme.controlBorderColor
        border.width: 1
        clip: true

        QfToolButton {
          width: viewSwitch.slotSize
          height: viewSwitch.slotSize
          anchors.centerIn: parent
          round: false
          hoverEnabled: false
          iconSource: viewSwitch.checked ? Theme.getThemeVectorIcon('ic_list_black_24dp') : Theme.getThemeVectorIcon('ic_grid_black_24dp')
          iconColor: Theme.mainTextColor
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

  function prepareFeature() {
    ensureEmbeddedFormLoaded();
    embeddedPopup.state = 'Add';
    embeddedPopup.currentLayer = relationEditorModel.relation.referencingLayer;
    embeddedPopup.linkedParentFeature = relationEditorModel.feature;
    embeddedPopup.linkedRelation = relationEditorModel.relation;
    if (relationEditorModel.orderingField) {
      embeddedPopup.linkedRelationOrderingField = relationEditorModel.orderingField;
    }
    embeddedPopup.attributeFormModel.applyParentDefaultValues();
    attachmentNamingEvaluator.feature = embeddedPopup.feature;
  }

  function showAddFeaturePopup(geometry, attachmentPath) {
    prepareFeature();
    if (geometry !== undefined) {
      embeddedPopup.applyGeometry(geometry);
    }

    const hasAttachmentPath = attachmentPath !== undefined && attachmentPath !== "";
    if (hasAttachmentPath && embeddedPopup.attributeFormModel.featureModel.suppressFeatureForm()) {
      embeddedPopup.featureModel.resetAttributes();
      embeddedPopup.attributeFormModel.changeAttribute(referencingFeatureListModel.attachmentFieldName, attachmentPath);
      embeddedPopup.confirmForm();
      return;
    }
    embeddedPopup.open();
    if (hasAttachmentPath) {
      embeddedPopup.attributeFormModel.changeAttribute(referencingFeatureListModel.attachmentFieldName, attachmentPath);
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
      if (FileUtils.mimeTypeName(path).startsWith("audio/")) {
        audioAnalyzer.enqueue(path);
      }
      return path;
    }
    const fullPath = imagePrefix + path;
    if (FileUtils.fileExists(fullPath)) {
      if (FileUtils.mimeTypeName(fullPath).startsWith("audio/")) {
        audioAnalyzer.enqueue(fullPath);
      }
      return fullPath;
    }
    if (fetchedPaths.hasOwnProperty(path)) {
      return fetchedPaths[path];
    }
    if (pendingDownloads.hasOwnProperty(path) || failedDownloads.hasOwnProperty(path)) {
      return "";
    }
    // File not found locally; attempt on-demand download
    if (externalStorage.type !== "") {
      const authConfigId = referencingFeatureListModel.attachmentStorageAuthConfigId;
      if (authConfigId !== "" && !iface.isAuthenticationConfigurationAvailable(authConfigId)) {
        failedDownloads[path] = true;
        mainWindow.displayToast(qsTr("The external storage's authentication configuration ID is missing, please insure it is imported into %1").arg(appName), "error", qsTr("Learn more"), function () {
          Qt.openUrlExternally('https://docs.qfield.org/how-to/advanced-how-tos/authentication/');
        });
      } else {
        pendingDownloads[path] = true;
        enqueueExternalFetch(path, referencingFeatureListModel.attachmentStorageUrl + path, authConfigId);
      }
    } else if (cloudProjectsModel && cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.attachmentsOnDemandEnabled) {
      pendingDownloads[path] = true;
      cloudProjectConnection.target = cloudProjectsModel.currentProject;
      cloudProjectsModel.currentProject.downloadAttachment(path);
    }
    return "";
  }

  Loader {
    id: relationCameraLoader
    active: false
    property bool isVideo: false
    sourceComponent: Component {
      QFieldCamera {
        allowCaptureModeToggle: true

        Component.onCompleted: {
          state = relationCameraLoader.isVideo ? 'VideoCapture' : 'PhotoCapture';
          open();
        }

        onFinished: path => {
          const filepath = StringUtils.replaceFilenameTags(ExternalResourceUtils.getAttachmentFilePath(attachmentNamingEvaluator.evaluate(), documentViewer, FileUtils), path);
          platformUtilities.renameFile(path, imagePrefix + filepath);
          if (!FileUtils.mimeTypeName(path).startsWith("video/")) {
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

      property string attachmentFullPath: ""
      Component.onCompleted: {
        attachmentFullPath = Qt.binding(() => resolveAttachmentPath(model.attachmentPath));
      }
      Connections {
        target: relationEditor
        function onDownloadRevisionChanged() {
          attachmentFullPath = Qt.binding(() => resolveAttachmentPath(model.attachmentPath));
        }
      }
      readonly property string attachmentMimeType: attachmentFullPath !== "" ? FileUtils.mimeTypeName(attachmentFullPath) : ""
      readonly property bool attachmentIsVideo: attachmentMimeType.startsWith("video/")
      readonly property bool attachmentIsAudio: attachmentMimeType.startsWith("audio/")
      readonly property bool attachmentIsImage: attachmentMimeType.startsWith("image/") && FileUtils.isImageMimeTypeSupported(attachmentMimeType)
      readonly property bool attachmentFetching: model.attachmentPath !== "" && attachmentFullPath === "" && relationEditor.pendingDownloads.hasOwnProperty(model.attachmentPath)

      Loader {
        id: listVideoThumbLoader
        active: false
        parent: listVideoContainer
        anchors.fill: parent

        property url sourceUrl: attachmentIsVideo ? UrlUtils.fromString(attachmentFullPath) : ""
        property bool firstFrameDrawn: false
        property bool queued: false

        onSourceUrlChanged: {
          active = false;
          firstFrameDrawn = false;
          queued = false;
          if (attachmentIsVideo && sourceUrl.toString() !== "") {
            queued = true;
            relationEditor.queueVideoLoad(() => {
              if (!listVideoThumbLoader || !listVideoThumbLoader.queued)
                return false;
              listVideoThumbLoader.queued = false;
              listVideoThumbLoader.active = true;
              return true;
            });
          }
        }

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

            Row {
              id: listWaveformBars
              anchors.centerIn: parent
              spacing: 2
              width: 56
              height: 56
              visible: attachmentIsAudio

              property int barCount: listWaveformRepeater.count
              property real barWidth: (listWaveformBars.width - (spacing * barCount)) / barCount

              Repeater {
                id: listWaveformRepeater
                model: attachmentFullPath in audioAnalyzer.availableBars ? audioAnalyzer.availableBars[attachmentFullPath] : [0]

                Rectangle {
                  width: listWaveformBars.barWidth
                  height: listWaveformBars.height * modelData
                  radius: 1.5
                  anchors.verticalCenter: parent.verticalCenter
                  color: Theme.mainTextDisabledColor
                  opacity: 0.35
                }
              }
            }

            //Document placeholder
            Item {
              anchors.centerIn: parent
              width: 32
              height: 32
              visible: !attachmentIsImage && !attachmentIsVideo && !attachmentIsAudio && attachmentFullPath !== ""

              Image {
                anchors.fill: parent
                source: Theme.getThemeVectorIcon("ic_file_black_24dp")
                fillMode: Image.PreserveAspectFit
                opacity: 0.3
              }

              Rectangle {
                anchors.centerIn: parent
                width: Math.min(listSuffixText.contentWidth + 6, parent.width)
                height: listSuffixText.contentHeight + 2
                radius: 2
                color: Theme.controlBorderColor
                visible: listSuffixText.text !== ""

                Text {
                  id: listSuffixText
                  anchors.centerIn: parent
                  text: FileUtils.fileSuffix(attachmentFullPath).toUpperCase()
                  font.pointSize: Theme.tinyFont.pointSize
                  font.weight: Font.Bold
                  color: Theme.mainTextColor
                  opacity: 0.6
                }
              }
            }

            // Empty attachment placeholder
            Image {
              anchors.centerIn: parent
              width: 28
              height: 28
              visible: !attachmentIsImage && !attachmentIsVideo && !attachmentIsAudio && !attachmentFetching && attachmentFullPath === ""
              source: Theme.getThemeVectorIcon("ic_photo_notavailable_black_24dp")
              fillMode: Image.PreserveAspectFit
              opacity: 0.3
            }

            BusyIndicator {
              anchors.centerIn: parent
              width: 32
              height: 32
              running: attachmentFetching
              visible: running
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
            width: listRow.width - listThumbnail.width - listFormButton.width - listMenuButton.width - listRow.spacing * 2
            text: model.displayString
            color: Theme.mainTextColor
            font: Theme.defaultFont
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
            maximumLineCount: 2
          }

          QfToolButton {
            id: listFormButton
            anchors.verticalCenter: parent.verticalCenter
            width: 48
            height: 48
            round: false
            iconSource: isEnabled ? Theme.getThemeVectorIcon('ic_edit_attributes_white_24dp') : Theme.getThemeVectorIcon('ic_baseline-list_white_24dp')
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'

            onClicked: openFeatureForm(model.referencingFeature, model.nmReferencedFeature)
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
          anchors.rightMargin: listMenuButton.width + listFormButton.width
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

      property string attachmentFullPath: ""
      Component.onCompleted: {
        attachmentFullPath = Qt.binding(() => resolveAttachmentPath(model.attachmentPath));
      }
      Connections {
        target: relationEditor
        function onDownloadRevisionChanged() {
          attachmentFullPath = Qt.binding(() => resolveAttachmentPath(model.attachmentPath));
        }
      }
      readonly property string attachmentMimeType: attachmentFullPath !== "" ? FileUtils.mimeTypeName(attachmentFullPath) : ""
      readonly property bool attachmentIsVideo: attachmentMimeType.startsWith("video/")
      readonly property bool attachmentIsAudio: attachmentMimeType.startsWith("audio/")
      readonly property bool attachmentIsImage: attachmentMimeType.startsWith("image/") && FileUtils.isImageMimeTypeSupported(attachmentMimeType)
      readonly property bool attachmentFetching: model.attachmentPath !== "" && attachmentFullPath === "" && relationEditor.pendingDownloads.hasOwnProperty(model.attachmentPath)

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
        active: false
        parent: cardVideoContainer
        anchors.fill: parent

        property url sourceUrl: attachmentIsVideo ? UrlUtils.fromString(attachmentFullPath) : ""
        property bool firstFrameDrawn: false
        property bool queued: false

        onSourceUrlChanged: {
          active = false;
          firstFrameDrawn = false;
          queued = false;
          if (attachmentIsVideo && sourceUrl.toString() !== "") {
            queued = true;
            relationEditor.queueVideoLoad(() => {
              if (!videoThumbLoader || !videoThumbLoader.queued)
                return false;
              videoThumbLoader.queued = false;
              videoThumbLoader.active = true;
              return true;
            });
          }
        }

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

            onPlaybackStateChanged: {
              // Detect natural end-of-playback
              if (cardContainer.videoPlaying && playbackState === MediaPlayer.StoppedState && duration > 0 && position >= duration * 0.1) {
                cardContainer.videoPlaying = false;
                relationEditor.releaseMediaFocus(cardContainer);
                seek(0);
                pause();
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
          anchors.fill: parent
          anchors.bottomMargin: detailsBar.height
          visible: attachmentIsAudio
          clip: true

          Row {
            id: gridWaveformBars
            anchors.centerIn: parent
            width: parent.width
            height: parent.height * 0.7
            spacing: 2

            property int barCount: gridWaveformRepeater.count
            property real barWidth: (gridWaveformBars.width - (spacing * barCount)) / barCount

            Repeater {
              id: gridWaveformRepeater

              model: attachmentFullPath in audioAnalyzer.availableBars ? audioAnalyzer.availableBars[attachmentFullPath] : [0]

              Rectangle {
                width: gridWaveformBars.barWidth
                height: gridWaveformBars.height * modelData
                radius: 1.5
                anchors.verticalCenter: parent.verticalCenter
                color: {
                  if (audioPlayerLoader.active && (index / gridWaveformBars.barCount) < audioPlayerLoader.progress) {
                    return Theme.mainColor;
                  }
                  return Theme.mainTextDisabledColor;
                }
                opacity: {
                  if (audioPlayerLoader.active && (index / gridWaveformBars.barCount) < audioPlayerLoader.progress) {
                    return 0.9;
                  }
                  return 0.35;
                }
              }
            }
          }

          Rectangle {
            visible: audioPlayerLoader.active && audioPlayerLoader.progress > 0
            x: gridWaveformBars.x + gridWaveformBars.width * audioPlayerLoader.progress
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

        //Document placeholder
        Item {
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.verticalCenter: parent.verticalCenter
          anchors.verticalCenterOffset: -(detailsBar.height / 2)
          width: 48
          height: 48
          visible: !attachmentIsImage && !attachmentIsVideo && !attachmentIsAudio && attachmentFullPath !== ""

          Image {
            anchors.fill: parent
            source: Theme.getThemeVectorIcon("ic_file_black_24dp")
            fillMode: Image.PreserveAspectFit
            opacity: 0.3
          }

          Rectangle {
            anchors.centerIn: parent
            width: Math.min(gridSuffixText.contentWidth + 8, parent.width)
            height: gridSuffixText.contentHeight + 4
            radius: 2
            color: Theme.controlBorderColor
            visible: gridSuffixText.text !== ""

            Text {
              id: gridSuffixText
              anchors.centerIn: parent
              text: FileUtils.fileSuffix(attachmentFullPath).toUpperCase()
              font.pointSize: Theme.tinyFont.pointSize
              font.weight: Font.Bold
              color: Theme.mainTextColor
              opacity: 0.6
            }
          }
        }

        // Empty attachment placeholder
        Image {
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.verticalCenter: parent.verticalCenter
          anchors.verticalCenterOffset: -(detailsBar.height / 2)
          width: 44
          height: 44
          visible: cardThumbnail.status !== Image.Ready && !attachmentIsVideo && !attachmentIsAudio && !attachmentFetching && attachmentFullPath === ""
          source: Theme.getThemeVectorIcon("ic_photo_notavailable_black_24dp")
          fillMode: Image.PreserveAspectFit
          opacity: 0.3
        }

        BusyIndicator {
          anchors.centerIn: parent
          anchors.verticalCenterOffset: -(detailsBar.height / 2)
          width: 36
          height: 36
          running: attachmentFetching
          visible: running
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
            font.pointSize: Theme.tipFont.pointSize
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
