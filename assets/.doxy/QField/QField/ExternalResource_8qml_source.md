

# File ExternalResource.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**ExternalResource.qml**](ExternalResource_8qml.md)

[Go to the documentation of this file](ExternalResource_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtMultimedia
import org.qgis
import org.qfield
import Theme
import ".."

EditorWidgetBase {
  anchors.left: parent.left
  anchors.right: parent.right

  height: childrenRect.height + 4

  ExpressionEvaluator {
    id: rootPathEvaluator
    project: qgisProject
    appExpressionContextScopesGenerator: appScopesGenerator
  }

  property string prefixToRelativePath: {
    if (qgisProject == undefined)
      return "";
    var path = "";
    if (config["RelativeStorage"] === 1 || externalStorage.type != "") {
      path = qgisProject.homePath;
      if (!path.endsWith("/"))
        path = path + "/";
    } else if (config["RelativeStorage"] === 2) {
      var collection = config["PropertyCollection"];
      var props = collection["properties"];
      if (props) {
        if (props["propertyRootPath"]) {
          var rootPathProps = props["propertyRootPath"];
          rootPathEvaluator.expressionText = rootPathProps["expression"];
        }
      }
      rootPathEvaluator.feature = currentFeature;
      rootPathEvaluator.layer = currentLayer;
      var evaluatedFilepath = rootPathEvaluator.evaluate().replace("\\", "/");
      if (evaluatedFilepath) {
        path = evaluatedFilepath;
      } else {
        path = config["DefaultRoot"] ? config["DefaultRoot"] : qgisProject.homePath;
        if (!path.endsWith("/"))
          path = path + "/";
      }
    }

    // since we've hardcoded the project path by default so far, let's maintain that until we improve things in qfieldsync
    if (path == "") {
      path = qgisProject.homePath;
      if (!path.endsWith("/"))
        path = path + "/";
    }
    return path;
  }

  property ResourceSource __resourceSource
  property ViewStatus __viewStatus

  // DocumentViewer values
  readonly property int document_FILE: 0
  readonly property int document_IMAGE: 1
  readonly property int document_WEB: 2 // TODO: implement
  readonly property int document_AUDIO: 3
  readonly property int document_VIDEO: 4
  property int documentViewer: config.DocumentViewer

  property bool isImage: false
  property bool isAudio: false
  property bool isVideo: false

  //to not break any binding of image.source
  property var currentValue: value
  onCurrentValueChanged: {
    if (currentValue != undefined && currentValue !== '') {
      const isHttp = value.startsWith('http://') || value.startsWith('https://');
      var fullValue = isHttp ? value : prefixToRelativePath + value;
      const fullValueExists = FileUtils.fileExists(fullValue);
      if (!fullValueExists) {
        prepareValue("");
        if (externalStorage.type != "") {
          if (config["StorageAuthConfigId"] !== "" && !iface.isAuthenticationConfigurationAvailable(config["StorageAuthConfigId"])) {
            mainWindow.displayToast(qsTr("The external storage's authentication configuration ID is missing, please insure it is imported into %1").arg(appName), "error", qsTr("Learn more"), function () {
              Qt.openUrlExternally('https://docs.qfield.org/how-to/advanced-how-tos/authentication/');
            });
          } else {
            externalStorage.fetch(value, config["StorageAuthConfigId"]);
            fetchingIndicator.running = true;
          }
        } else if (cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.attachmentsOnDemandEnabled) {
          cloudProjectConnection.target = cloudProjectsModel.currentProject;
          cloudProjectConnection.downloadAttachmentFileName = value;
          cloudProjectsModel.currentProject.downloadAttachment(value);
          fetchingIndicator.running = true;
        }
      } else {
        prepareValue(fullValue);
      }
    } else {
      prepareValue("");
    }
  }

  function prepareValue(fullValue) {
    if (fullValue !== "") {
      var mimeType = FileUtils.mimeTypeName(fullValue);
      isImage = !config.UseLink && mimeType.startsWith("image/") && FileUtils.isImageMimeTypeSupported(mimeType);
      isAudio = !config.UseLink && mimeType.startsWith("audio/");
      isVideo = !config.UseLink && mimeType.startsWith("video/");
      image.visible = isImage;
      geoTagBadge.visible = isImage;
      if (isImage) {
        mediaFrame.height = 200;
        image.visible = true;
        image.hasImage = true;
        image.opacity = 1;
        image.anchors.topMargin = 0;
        image.source = UrlUtils.fromString(fullValue);
        geoTagBadge.hasGeoTag = ExifTools.hasGeoTag(fullValue);
      } else if (isAudio || isVideo) {
        mediaFrame.height = 48;
        image.visible = false;
        image.opacity = 0.5;
        image.source = '';
        player.firstFrameDrawn = false;
        player.sourceUrl = UrlUtils.fromString(fullValue);
      }
    } else {
      image.source = '';
      image.visible = documentViewer == document_IMAGE;
      image.opacity = 0.15;
      geoTagBadge.visible = false;
      player.sourceUrl = '';
    }
  }

  ExternalStorage {
    id: externalStorage
    type: config["StorageType"] !== undefined ? config["StorageType"] : ""

    onFetchedContentChanged: {
      fetchingIndicator.running = false;
      prepareValue(fetchedContent);
    }

    onLastErrorChanged: {
      fetchingIndicator.running = false;
      mainWindow.displayToast(lastError, "error");
    }
  }

  ExpressionEvaluator {
    id: expressionEvaluator
    feature: currentFeature
    layer: currentLayer
    project: qgisProject
    appExpressionContextScopesGenerator: appScopesGenerator
    expressionText: {
      var value;
      if (currentLayer && currentLayer.customProperty('QFieldSync/attachment_naming') !== undefined) {
        value = JSON.parse(currentLayer.customProperty('QFieldSync/attachment_naming'))[field.name];
        return value !== undefined ? value : '';
      } else if (currentLayer && currentLayer.customProperty('QFieldSync/photo_naming') !== undefined) {
        // Fallback to old configuration key
        value = JSON.parse(currentLayer.customProperty('QFieldSync/photo_naming'))[field.name];
        return value !== undefined ? value : '';
      }
      return '';
    }
  }

  function getResourceFilePath() {
    const evaluatedFilepath = expressionEvaluator.evaluate();
    let filepath = FileUtils.sanitizeFilePath(evaluatedFilepath);

    // assume that lack of file suffix means problem with the filepath evaluation, so last resort to fallback filepaths.
    // the fallback filepaths are assumed to be always correct and are not passed through `FileUtils.sanitizeFilePath`.
    if (FileUtils.fileSuffix(filepath) === '' && !filepath.endsWith("{extension}") && !filepath.endsWith("{filename}")) {
      // the `nosStr` stores the current datetime as single numeric string, e.g. 20250101234589
      const nowStr = (new Date()).toISOString().replace(/[^0-9]/g, '');

      // we need an extension for media types (image, audio, video), fallback to hardcoded values, the `{extension} and `{filename} are placeholders that should be replaced by the caller. See `StringUtils::replaceFilenameTags()`.
      if (documentViewer == document_IMAGE) {
        filepath = `DCIM/JPEG_${nowStr}.{extension}`;
      } else if (documentViewer == document_AUDIO) {
        filepath = `audio/AUDIO_${nowStr}.{extension}`;
      } else if (documentViewer == document_VIDEO) {
        filepath = `video/VIDEO_${nowStr}.{extension}`;
      } else {
        filepath = `files/${nowStr}_{filename}`;
      }
    }

    // while `FileUtils.sanitizeFilePath` should have fixed the slashes, we redo it just in case the fallback filepaths are wrong. Note that `QFile` always expects UNIX style slashes.
    filepath = filepath.replace('\\', '/');
    return filepath;
  }

  Label {
    id: linkField

    property bool hasValue: currentValue !== undefined && currentValue != ""
    property bool isVisible: hasValue && !isImage && !isAudio && !isVideo && !fetchingIndicator.running

    topPadding: 10
    bottomPadding: 10
    height: fontMetrics.height + 30
    visible: isVisible

    anchors.left: parent.left
    anchors.right: cameraButton.left
    color: {
      if ((!isEditable && isEditing) || isNull || isEmpty) {
        return Theme.mainTextDisabledColor;
      }
      return FileUtils.fileExists(prefixToRelativePath + value) ? Theme.mainColor : Theme.secondaryTextColor;
    }

    text: {
      if (isEmpty) {
        return qsTr("Empty");
      } else if (isNull) {
        return qsTr("NULL");
      }
      let fieldValue = qsTr('No Value');
      if (hasValue) {
        fieldValue = prefixToRelativePath + currentValue;
        if (UrlUtils.isRelativeOrFileUrl(fieldValue)) {
          fieldValue = config.FullUrl ? fieldValue : FileUtils.fileName(fieldValue);
        }
        fieldValue = StringUtils.insertLinks(fieldValue);
      }
      return fieldValue;
    }

    font.pointSize: Theme.defaultFont.pointSize
    font.italic: !hasValue
    font.underline: FileUtils.fileExists(prefixToRelativePath + value) || FileUtils.fileExists(value)
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideMiddle

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if (!value)
          return;
        if (!UrlUtils.isRelativeOrFileUrl(value)) {
          // matches `http://...` but not `file://...` paths
          Qt.openUrlExternally(value);
        } else if (FileUtils.fileExists(prefixToRelativePath + value)) {
          __viewStatus = platformUtilities.open(prefixToRelativePath + value, isEnabled, this);
        }
      }
    }
  }

  FontMetrics {
    id: fontMetrics
    font: linkField.font
  }

  Rectangle {
    id: mediaFrame
    width: parent.width - fileButton.width - cameraButton.width - cameraVideoButton.width - microphoneButton.width - (isEnabled ? 5 : 0)
    height: 48
    visible: !linkField.isVisible
    color: Theme.controlBorderColor
    radius: 5
    clip: true

    Rectangle {
      id: roundMask
      anchors.fill: parent
      anchors.margins: 1
      radius: mediaFrame.radius
      color: "white"
      visible: false
      layer.enabled: true
    }

    BusyIndicator {
      id: fetchingIndicator
      anchors.centerIn: parent
      anchors.margins: 5
      width: parent.height
      height: parent.height
      running: false
      visible: running
    }

    Image {
      id: image

      property bool hasImage: false

      visible: isImage
      enabled: isImage
      anchors.centerIn: parent
      width: hasImage ? parent.width : 24
      height: hasImage ? parent.height : 24
      opacity: 0.25
      autoTransform: true
      fillMode: Image.PreserveAspectFit
      horizontalAlignment: Image.AlignHCenter
      verticalAlignment: Image.AlignVCenter

      source: Theme.getThemeVectorIcon("ic_photo_notavailable_black_24dp")
      cache: false

      layer.enabled: true
      layer.effect: QfOpacityMask {
        maskSource: roundMask
      }

      Image {
        id: geoTagBadge
        property bool hasGeoTag: false
        visible: false
        anchors.bottom: image.bottom
        anchors.right: image.right
        anchors.rightMargin: 10
        anchors.bottomMargin: 12
        fillMode: Image.PreserveAspectFit
        width: 24
        height: 24
        source: hasGeoTag ? Theme.getThemeVectorIcon("ic_geotag_white_24dp") : Theme.getThemeVectorIcon("ic_geotag_missing_white_24dp")
        sourceSize.width: 24 * Screen.devicePixelRatio
        sourceSize.height: 24 * Screen.devicePixelRatio
      }

      QfDropShadow {
        anchors.fill: geoTagBadge
        visible: geoTagBadge.visible
        horizontalOffset: 0
        verticalOffset: 0
        radius: 6.0
        color: "#DD000000"
        source: geoTagBadge
      }
    }

    Loader {
      id: player
      active: isAudio || isVideo

      property url sourceUrl: ''
      property bool firstFrameDrawn: false

      anchors.left: parent.left
      anchors.top: parent.top

      width: parent.width
      height: parent.height - 54

      sourceComponent: Component {
        Video {
          visible: isVideo
          anchors.fill: parent

          muted: false
          volume: 1.0
          source: player.sourceUrl

          onHasVideoChanged: {
            mediaFrame.height = hasVideo ? 254 : 48;
            if (!player.firstFrameDrawn && hasVideo) {
              play();
            }
          }

          onDurationChanged: {
            positionSlider.to = duration / 1000;
            positionSlider.value = 0;
            if (!player.firstFrameDrawn && hasVideo) {
              play();
            }
          }

          onPositionChanged: {
            if (!player.firstFrameDrawn && playbackState === MediaPlayer.PlayingState) {
              player.firstFrameDrawn = true;
              if (hasVideo) {
                pause();
              }
            }
            positionSlider.value = position / 1000;
          }
        }
      }
    }

    MouseArea {
      enabled: mediaFrame.visible
      width: parent.width
      height: playerControls.visible ? player.height - 54 : image.height

      onClicked: {
        if (FileUtils.fileExists(prefixToRelativePath + value)) {
          platformUtilities.requestStoragePermission();
          __viewStatus = platformUtilities.open(prefixToRelativePath + value, isEnabled, this);
        }
      }
    }

    QfToolButton {
      id: sketchButton
      anchors.top: image.top
      anchors.right: image.right
      visible: image.source != '' && image.status === Image.Ready && isEnabled

      round: true
      iconSource: Theme.getThemeVectorIcon("ic_freehand_white_24dp")
      iconColor: Theme.toolButtonColor
      bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor

      onClicked: {
        sketcherConnection.enabled = true;
        sketcher.loadImage(image.source);
        sketcher.open();
      }
    }

    Connections {
      id: sketcherConnection
      target: sketcher
      enabled: false

      function onFinished(path) {
        const filepath = StringUtils.replaceFilenameTags(getResourceFilePath(), path);
        platformUtilities.renameFile(path, prefixToRelativePath + filepath);

        // In order to insure an edited image gets refreshed in the feature form, reset the source
        image.source = '';
        image.source = UrlUtils.fromString(prefixToRelativePath + filepath);
        valueChangeRequested(filepath, false);
        enabled = false;
      }

      function onCancelled() {
        enabled = false;
      }
    }

    RowLayout {
      id: playerControls

      visible: player.active && player.item.duration > 0

      anchors.left: parent.left
      anchors.bottom: parent.bottom
      anchors.leftMargin: 5
      anchors.rightMargin: 5
      width: parent.width - 10

      QfToolButton {
        id: playButton

        iconSource: player.active && player.item.playbackState === MediaPlayer.PlayingState ? Theme.getThemeVectorIcon('ic_pause_black_24dp') : Theme.getThemeVectorIcon('ic_play_black_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"

        onClicked: {
          if (player.item.playbackState === MediaPlayer.PlayingState) {
            player.item.pause();
          } else {
            player.item.play();
          }
        }
      }

      Slider {
        id: positionSlider
        Layout.fillWidth: true

        from: 0
        to: 0

        enabled: to > 0

        onMoved: {
          player.item.seek(value * 1000);
        }
      }

      Label {
        id: durationLabel
        Layout.preferredWidth: durationLabelMetrics.boundingRect('00:00:00').width
        Layout.rightMargin: 14

        color: player.active && player.item.playbackState === MediaPlayer.PlayingState ? Theme.mainTextColor : Theme.mainTextDisabledColor
        font: Theme.tipFont
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: {
          if (player.active && player.item.duration > 0) {
            var seconds = Math.ceil(player.item.duration / 1000);
            var hours = Math.floor(seconds / 60 / 60) + '';
            seconds -= hours * 60 * 60;
            var minutes = Math.floor(seconds / 60) + '';
            seconds = (seconds - minutes * 60) + '';
            return hours.padStart(2, '0') + ':' + minutes.padStart(2, '0') + ':' + seconds.padStart(2, '0');
          } else {
            return '-';
          }
        }
      }

      FontMetrics {
        id: durationLabelMetrics
        font: durationLabel.font
      }
    }
  }

  QfToolButton {
    id: cameraButton
    width: visible ? 48 : 0
    height: 48

    // QField has historically handled no viewer type as image, let's carry that on
    visible: documentViewer == document_IMAGE && isEnabled

    anchors.right: cameraVideoButton.left
    anchors.top: parent.top

    iconSource: Theme.getThemeVectorIcon("ic_camera_photo_black_24dp")
    iconColor: Theme.mainTextColor
    bgcolor: "transparent"

    onClicked: capturePhoto()
  }

  QfToolButton {
    id: cameraVideoButton
    width: visible ? 48 : 0
    height: 48

    visible: documentViewer == document_VIDEO && isEnabled

    anchors.right: microphoneButton.left
    anchors.top: parent.top

    iconSource: Theme.getThemeVectorIcon("ic_camera_video_black_24dp")
    iconColor: Theme.mainTextColor
    bgcolor: "transparent"

    onClicked: captureVideo()
  }

  QfToolButton {
    id: microphoneButton
    width: visible ? 48 : 0
    height: 48

    visible: documentViewer == document_AUDIO && isEnabled

    anchors.right: fileButton.left
    anchors.top: parent.top

    iconSource: Theme.getThemeVectorIcon("ic_microphone_black_24dp")
    iconColor: Theme.mainTextColor
    bgcolor: "transparent"

    onClicked: captureAudio()
  }

  QfToolButton {
    id: fileButton
    width: visible ? 48 : 0
    height: 48

    visible: platformUtilities.capabilities & PlatformUtilities.FilePicker && documentViewer == document_FILE && isEnabled

    anchors.right: parent.right
    anchors.top: parent.top

    iconSource: Theme.getThemeVectorIcon("ic_file_black_24dp")
    iconColor: Theme.mainTextColor
    bgcolor: "transparent"

    onClicked: attachFile()
  }

  Loader {
    id: audioRecorderLoader
    sourceComponent: audioRecorderComponent
    active: false
  }

  Loader {
    id: cameraLoader
    property bool isVideo: false
    sourceComponent: cameraComponent
    active: false
  }

  Component {
    id: audioRecorderComponent

    QFieldAudioRecorder {
      z: 10000
      visible: false

      Component.onCompleted: {
        open();
      }

      onFinished: path => {
        const filepath = StringUtils.replaceFilenameTags(getResourceFilePath(), path);
        console.log("path " + path);
        console.log("filepath " + filepath);
        console.log("prefixToRelativePath + filepath " + prefixToRelativePath + filepath);
        platformUtilities.renameFile(path, prefixToRelativePath + filepath);
        valueChangeRequested(filepath, false);
        close();
      }

      onCanceled: {
        close();
      }

      onClosed: {
        audioRecorderLoader.active = false;
      }
    }
  }

  Component {
    id: cameraComponent

    QFieldCamera {
      id: qfieldCamera
      visible: false

      Component.onCompleted: {
        if (isVideo) {
          qfieldCamera.state = 'VideoCapture';
          open();
        } else {
          qfieldCamera.state = 'PhotoCapture';
          open();
        }
      }

      onFinished: path => {
        const filepath = StringUtils.replaceFilenameTags(getResourceFilePath(), path);
        platformUtilities.renameFile(path, prefixToRelativePath + filepath);
        if (!cameraLoader.isVideo) {
          const maximumWidhtHeight = iface.readProjectNumEntry("qfieldsync", "maximumImageWidthHeight", 0);
          if (maximumWidhtHeight > 0) {
            FileUtils.restrictImageSize(prefixToRelativePath + filepath, maximumWidhtHeight);
          }
        }
        valueChangeRequested(filepath, false);
        close();
      }

      onCanceled: {
        close();
      }

      onClosed: {
        cameraLoader.active = false;
      }
    }
  }

  Connections {
    target: __resourceSource
    function onResourceReceived(path) {
      if (path) {
        var maximumWidhtHeight = iface.readProjectNumEntry("qfieldsync", "maximumImageWidthHeight", 0);
        if (maximumWidhtHeight > 0) {
          FileUtils.restrictImageSize(prefixToRelativePath + path, maximumWidhtHeight);
        }
        valueChangeRequested(path, false);
      }
    }
  }

  Connections {
    target: __viewStatus

    function onFinished() {
      if (isImage) {
        // In order to make sure the image shown reflects edits, reset the source
        var imageSource = image.source;
        image.source = '';
        image.source = imageSource;
      }
    }

    function onStatusReceived(statusText) {
      if (statusText !== "") {
        displayToast(qsTr("Cannot handle this file type"), 'error');
      }
    }
  }

  QtObject {
    id: dummyTarget
  }
  Connections {
    id: cloudProjectConnection
    ignoreUnknownSignals: true
    target: dummyTarget

    property string downloadAttachmentFileName: ""

    function onDownloadAttachmentFinished(fileName, errorString) {
      if (downloadAttachmentFileName === fileName) {
        if (errorString !== "") {
          displayToast(qsTr("QFieldCloud on-demand attachment error: ") + errorString, 'error');
        } else {
          prepareValue(prefixToRelativePath + fileName);
        }
        cloudProjectConnection.target = dummyTarget;
        fetchingIndicator.running = false;
      }
    }
  }

  function attachFile() {
    Qt.inputMethod.hide();
    platformUtilities.requestStoragePermission();
    var filepath = getResourceFilePath();
    if (documentViewer == document_AUDIO) {
      __resourceSource = platformUtilities.getFile(qgisProject.homePath + '/', filepath, PlatformUtilities.AudioFiles, this);
    } else {
      __resourceSource = platformUtilities.getFile(qgisProject.homePath + '/', filepath, this);
    }
  }

  function attachGallery() {
    Qt.inputMethod.hide();
    platformUtilities.requestStoragePermission();
    var filepath = getResourceFilePath();
    if (documentViewer == document_VIDEO) {
      __resourceSource = platformUtilities.getGalleryVideo(qgisProject.homePath + '/', filepath, this);
    } else {
      __resourceSource = platformUtilities.getGalleryPicture(qgisProject.homePath + '/', filepath, this);
    }
  }

  function capturePhoto() {
    Qt.inputMethod.hide();
    if (platformUtilities.capabilities & PlatformUtilities.NativeCamera && settings.valueBool("nativeCamera2", true)) {
      var filepath = getResourceFilePath();
      // Pictures taken by cameras will always be JPG
      filepath = filepath.replace('{extension}', 'JPG');
      __resourceSource = platformUtilities.getCameraPicture(qgisProject.homePath + '/', filepath, FileUtils.fileSuffix(filepath), this);
    } else {
      platformUtilities.createDir(qgisProject.homePath, 'DCIM');
      cameraLoader.isVideo = false;
      cameraLoader.active = true;
    }
  }

  function captureVideo() {
    Qt.inputMethod.hide();
    if (platformUtilities.capabilities & PlatformUtilities.NativeCamera && settings.valueBool("nativeCamera2", true)) {
      var filepath = getResourceFilePath();
      // Video taken by cameras will always be MP4
      filepath = filepath.replace('{extension}', 'MP4');
      __resourceSource = platformUtilities.getCameraVideo(qgisProject.homePath + '/', filepath, FileUtils.fileSuffix(filepath), this);
    } else {
      platformUtilities.createDir(qgisProject.homePath, 'DCIM');
      cameraLoader.isVideo = true;
      cameraLoader.active = true;
    }
  }

  function captureAudio() {
    Qt.inputMethod.hide();
    audioRecorderLoader.active = true;
  }

  Component.onCompleted: {
    menu.addItem(capturePhotoMenuItem);
    menu.addItem(captureVideoMenuItem);
    menu.addItem(captureAudioMenuItem);
    menu.addItem(separatorGalleryItem);
    if (platformUtilities.capabilities & PlatformUtilities.FilePicker) {
      menu.addItem(attachFileMenuItem);
    }
    menu.addItem(attachGalleryMenuItem);
    menu.addItem(separatorDrawingItem);
    menu.addItem(attachDrawingMenuItem);
    hasMenu = true;
  }

  Item {
    visible: false

    MenuItem {
      id: capturePhotoMenuItem
      text: qsTr('Take a photo')

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_camera_photo_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: capturePhoto()
    }

    MenuItem {
      id: captureVideoMenuItem
      text: qsTr('Take a video')

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_camera_video_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: captureVideo()
    }

    MenuItem {
      id: captureAudioMenuItem
      text: qsTr('Record an audio clip')

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_microphone_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: captureAudio()
    }

    MenuSeparator {
      id: separatorGalleryItem
      width: parent.width
    }

    MenuItem {
      id: attachGalleryMenuItem
      text: qsTr('Attach a gallery item')

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_gallery_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: attachGallery()
    }

    MenuItem {
      id: attachFileMenuItem
      text: qsTr('Attach a file')

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_file_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: attachFile()
    }

    MenuSeparator {
      id: separatorDrawingItem
      width: parent.width
    }

    MenuItem {
      id: attachDrawingMenuItem
      text: qsTr('Draw a sketch')

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_freehand_white_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        sketcherConnection.enabled = true;
        sketcher.clear();
        sketcher.open();
      }
    }
  }
}
```


