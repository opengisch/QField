import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCore
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Page {
  signal finished

  property alias currentPanel: bar.currentIndex

  property alias showScaleBar: registry.showScaleBar
  property alias showZoomControls: registry.showZoomControls
  property alias fullScreenIdentifyView: registry.fullScreenIdentifyView
  property alias locatorKeepScale: registry.locatorKeepScale
  property alias autoOpenFormSingleIdentify: registry.autoOpenFormSingleIdentify
  property alias autoZoomToIdentifiedFeature: registry.autoZoomToIdentifiedFeature
  property alias numericalDigitizingInformation: registry.numericalDigitizingInformation
  property alias showBookmarks: registry.showBookmarks
  property alias nativeCamera2: registry.nativeCamera2
  property alias digitizingVolumeKeys: registry.digitizingVolumeKeys
  property alias autoSave: registry.autoSave
  property alias fingerTapDigitizing: registry.fingerTapDigitizing
  property alias mouseAsTouchScreen: registry.mouseAsTouchScreen
  property alias enableInfoCollection: registry.enableInfoCollection
  property alias enableMapRotation: registry.enableMapRotation
  property alias quality: registry.quality
  property alias previewJobsEnabled: registry.previewJobsEnabled
  property alias snapToCommonAngleIsEnabled: registry.snapToCommonAngleIsEnabled
  property alias snapToCommonAngleIsRelative: registry.snapToCommonAngleIsRelative
  property alias snapToCommonAngleDegrees: registry.snapToCommonAngleDegrees
  property alias snapToCommonAngleTolerance: registry.snapToCommonAngleTolerance

  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  visible: false
  focus: visible

  Component.onCompleted: {
    if (settings.valueBool('nativeCameraLaunched', false)) {
      // a crash occured while the native camera was launched, disable it
      nativeCamera2 = false;
    }
  }

  function reset() {
    variableEditor.reset();
  }

  Settings {
    id: registry
    property bool showScaleBar: true
    property bool showZoomControls: false
    property bool fullScreenIdentifyView: false
    property bool locatorKeepScale: false
    property bool autoOpenFormSingleIdentify: false
    property bool autoZoomToIdentifiedFeature: false
    property bool numericalDigitizingInformation: false
    property bool showBookmarks: true
    property bool nativeCamera2: false
    property bool digitizingVolumeKeys: platformUtilities.capabilities & PlatformUtilities.VolumeKeys
    property bool autoSave: false
    property bool fingerTapDigitizing: false
    property bool mouseAsTouchScreen: false
    property bool enableInfoCollection: true
    property bool enableMapRotation: true
    property double quality: 1.0
    property bool previewJobsEnabled: true

    property bool snapToCommonAngleIsEnabled: false
    property bool snapToCommonAngleIsRelative: true
    property double snapToCommonAngleDegrees: 45.0// = settings.valueInt("/QField/Digitizing/SnapToCommonAngleDegrees", 45);
    property int snapToCommonAngleTolerance: 1// = settings.valueInt("/QField/Digitizing/SnappingTolerance", 1);

    onEnableInfoCollectionChanged: {
      if (enableInfoCollection) {
        iface.initiateSentry();
      } else {
        iface.closeSentry();
      }
    }

    onDigitizingVolumeKeysChanged: {
      platformUtilities.setHandleVolumeKeys(digitizingVolumeKeys && stateMachine.state != 'browse');
    }

    onFingerTapDigitizingChanged: {
      coordinateLocator.sourceLocation = undefined;
    }
  }

  ListModel {
    id: canvasSettingsModel
    ListElement {
      title: qsTr("Show scale bar")
      description: ''
      settingAlias: "showScaleBar"
      isVisible: true
    }
    ListElement {
      title: qsTr("Show zoom controls")
      description: ''
      settingAlias: "showZoomControls"
      isVisible: true
    }
    ListElement {
      title: qsTr("Show bookmarks")
      description: qsTr("When switched on, user's saved and currently opened project bookmarks will be displayed on the map.")
      settingAlias: "showBookmarks"
      isVisible: true
    }
    ListElement {
      title: qsTr("Enable map rotation")
      description: qsTr("When switched on, the map can be rotated by the user.")
      settingAlias: "enableMapRotation"
      isVisible: true
    }
  }

  ListModel {
    id: digitizingEditingSettingsModel
    ListElement {
      title: qsTr("Show digitizing information")
      description: qsTr("When switched on, coordinate information, such as latitude and longitude, is overlayed onto the map while digitizing new features or using the measure tool.")
      settingAlias: "numericalDigitizingInformation"
      isVisible: true
    }
    ListElement {
      title: qsTr("Use volume keys to digitize")
      description: qsTr("If enabled, pressing the device's volume up key will add a vertex while pressing volume down key will remove the last entered vertex during digitizing sessions.")
      settingAlias: "digitizingVolumeKeys"
      isVisible: true
    }
    ListElement {
      title: qsTr("Allow finger tap on canvas to add vertices")
      description: qsTr("When enabled, tapping on the map canvas with a finger will add a vertex at the tapped location.")
      settingAlias: "fingerTapDigitizing"
      isVisible: true
    }
    ListElement {
      title: qsTr("Consider mouse as a touchscreen device")
      description: qsTr("When enabled, the mouse will act as if it was a finger. When disabled, the mouse will match the stylus behavior.")
      settingAlias: "mouseAsTouchScreen"
      isVisible: true
    }
    Component.onCompleted: {
      for (var i = 0; i < count; i++) {
        if (get(i).settingAlias === 'digitizingVolumeKeys') {
          setProperty(i, 'isVisible', platformUtilities.capabilities & PlatformUtilities.VolumeKeys ? true : false);
        } else {
          setProperty(i, 'isVisible', true);
        }
      }
    }
  }

  ListModel {
    id: interfaceSettingsModel
    ListElement {
      title: qsTr("Maximize feature form")
      description: ''
      settingAlias: "fullScreenIdentifyView"
      isVisible: true
    }
    ListElement {
      title: qsTr("Open feature form for single feature identification")
      description: qsTr("When enabled, the feature form will open automatically if only one feature is identified, skipping the feature list.")
      settingAlias: "autoOpenFormSingleIdentify"
      isVisible: true
    }
    ListElement {
      title: qsTr("Fixed scale navigation")
      description: qsTr("When fixed scale navigation is active, focusing on a search result will pan to the feature. With fixed scale navigation disabled it will pan and zoom to the feature.")
      settingAlias: "locatorKeepScale"
      isVisible: true
    }
    ListElement {
      title: qsTr("Auto-zoom to identified feature(s)")
      description: qsTr("When enabled, the map will automatically zoom to show all identified features, as well as the individual selected feature when the feature form is opened.")
      settingAlias: "autoZoomToIdentifiedFeature"
      isVisible: true
    }
  }

  ListModel {
    id: advancedSettingsModel
    ListElement {
      title: qsTr("Render preview content around visible map canvas")
      description: qsTr("If enabled, areas just outside of the visible map canvas extent will be partially rendered to allow preview when zooming and panning.")
      settingAlias: "previewJobsEnabled"
      isVisible: true
    }
    ListElement {
      title: qsTr("Enable auto-save mode")
      description: qsTr("If enabled, newly-added features are stored as soon as it has having a valid geometry and the constraints are fulfilled and edited atributes are commited immediately.")
      settingAlias: "autoSave"
      isVisible: true
    }
    ListElement {
      title: qsTr("Use native camera")
      description: qsTr("If enabled, anonymized metrics will be collected and sent to help improve the experience for everyone.")
      settingAlias: "nativeCamera2"
      isVisible: true
    }
    ListElement {
      title: qsTr("Send anonymized metrics")
      description: qsTr("If enabled, anonymized metrics will be collected and sent to help improve %1 for everyone.")
      settingAlias: "enableInfoCollection"
      isVisible: true
    }
    Component.onCompleted: {
      for (var i = 0; i < count; i++) {
        if (get(i).settingAlias === 'nativeCamera2') {
          setProperty(i, 'isVisible', platformUtilities.capabilities & PlatformUtilities.NativeCamera ? true : false);
        } else if (get(i).settingAlias === 'enableInfoCollection') {
          setProperty(i, 'isVisible', platformUtilities.capabilities & PlatformUtilities.SentryFramework ? true : false);
        } else {
          setProperty(i, 'isVisible', true);
        }
      }
    }
  }

  Rectangle {
    color: Theme.mainBackgroundColor
    anchors.fill: parent
  }

  ColumnLayout {
    id: barColumn
    anchors {
      top: parent.top
      left: parent.left
      right: parent.right
      bottom: parent.bottom
    }

    QfTabBar {
      id: bar
      model: [qsTr("General"), qsTr("Positioning"), qsTr("Variables")]
      Layout.fillWidth: true
      Layout.preferredHeight: defaultHeight
    }

    Component {
      id: listItem

      Rectangle {
        width: parent ? parent.width - 16 : undefined
        height: isVisible ? line.height : 0
        color: "transparent"
        clip: true

        Row {
          id: line
          width: parent.width

          Column {
            width: parent.width - toggle.width

            Label {
              width: parent.width
              padding: 8
              leftPadding: 20
              text: title
              font: Theme.defaultFont
              color: Theme.mainTextColor
              wrapMode: Text.WordWrap
              MouseArea {
                anchors.fill: parent
                onClicked: toggle.toggle()
              }
            }

            Label {
              width: parent.width
              visible: description !== ''
              padding: description !== '' ? 8 : 0
              topPadding: 0
              leftPadding: 20
              text: description
              font: Theme.tipFont
              color: Theme.secondaryTextColor
              wrapMode: Text.WordWrap
            }
          }

          QfSwitch {
            id: toggle
            width: implicitContentWidth
            checked: registry[settingAlias]
            Layout.alignment: Qt.AlignTop | Qt.AlignRight
            onCheckedChanged: registry[settingAlias] = checked
          }
        }
      }
    }

    SwipeView {
      id: swipeView
      currentIndex: bar.currentIndex
      Layout.fillHeight: true
      Layout.fillWidth: true
      onCurrentIndexChanged: bar.currentIndex = swipeView.currentIndex
      clip: true

      Item {
        ScrollView {
          topPadding: 5
          leftPadding: 0
          rightPadding: 0
          ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
          ScrollBar.vertical: QfScrollBar {}
          contentWidth: generalSettingsGrid.width
          contentHeight: generalSettingsGrid.height
          anchors.fill: parent
          clip: true

          ColumnLayout {
            id: generalSettingsGrid
            width: swipeView.width

            GridLayout {
              Layout.fillWidth: true
              Layout.leftMargin: 20
              Layout.rightMargin: 20

              columns: 2
              columnSpacing: 0
              rowSpacing: 5

              Label {
                text: qsTr('Map Canvas')
                font: Theme.strongFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.topMargin: 5
                Layout.columnSpan: 2
              }
            }

            ListView {
              Layout.fillWidth: true
              Layout.preferredHeight: childrenRect.height
              interactive: false

              model: canvasSettingsModel

              delegate: listItem
            }

            GridLayout {
              Layout.fillWidth: true
              Layout.leftMargin: 20
              Layout.rightMargin: 20
              Layout.bottomMargin: 0

              columns: 2
              columnSpacing: 0
              rowSpacing: 5

              Label {
                Layout.fillWidth: true
                Layout.columnSpan: 2
                text: qsTr("Map canvas rendering quality:")
                font: Theme.defaultFont
                color: Theme.mainTextColor

                wrapMode: Text.WordWrap
              }

              QfComboBox {
                id: renderingQualityComboBox
                enabled: true
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter
                font: Theme.defaultFont

                popup.font: Theme.defaultFont
                popup.topMargin: mainWindow.sceneTopMargin
                popup.bottomMargin: mainWindow.sceneTopMargin

                model: ListModel {
                  ListElement {
                    name: qsTr('Best quality')
                    value: 1.0
                  }
                  ListElement {
                    name: qsTr('Lower quality')
                    value: 0.75
                  }
                  ListElement {
                    name: qsTr('Lowest quality')
                    value: 0.5
                  }
                }
                textRole: "name"
                valueRole: "value"

                property bool initialized: false

                onCurrentValueChanged: {
                  if (initialized) {
                    quality = currentValue;
                  }
                }

                Component.onCompleted: {
                  currentIndex = indexOfValue(quality);
                  initialized = true;
                }
              }

              Label {
                text: qsTr("A lower quality trades rendering precision in favor of lower memory usage and rendering time.")
                font: Theme.tipFont
                color: Theme.secondaryTextColor
                textFormat: Qt.RichText
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.columnSpan: 2

                onLinkActivated: link => {
                  Qt.openUrlExternally(link);
                }
              }

              Label {
                text: qsTr('Digitizing & Editing')
                font: Theme.strongFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.topMargin: 10
                Layout.columnSpan: 2
              }
            }

            ListView {
              Layout.fillWidth: true
              Layout.preferredHeight: childrenRect.height
              interactive: false

              model: digitizingEditingSettingsModel

              delegate: listItem
            }

            GridLayout {
              Layout.fillWidth: true
              Layout.leftMargin: 20
              Layout.rightMargin: 20
              Layout.bottomMargin: 0

              columns: 2
              columnSpacing: 0
              rowSpacing: 0

              Label {
                text: qsTr('User Interface')
                font: Theme.strongFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.topMargin: 5
                Layout.columnSpan: 2
              }

              Label {
                text: qsTr("Customize search bar")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.topMargin: 5

                MouseArea {
                  anchors.fill: parent
                  onClicked: showSearchBarSettings.clicked()
                }
              }

              QfToolButton {
                id: showSearchBarSettings
                Layout.preferredWidth: 48
                Layout.preferredHeight: 48
                Layout.alignment: Qt.AlignVCenter
                clip: true

                iconSource: Theme.getThemeVectorIcon("ic_ellipsis_black_24dp")
                iconColor: Theme.mainColor
                bgcolor: "transparent"

                onClicked: {
                  locatorSettings.open();
                  locatorSettings.focus = true;
                }
              }

              Label {
                text: qsTr("Manage plugins")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true

                MouseArea {
                  anchors.fill: parent
                  onClicked: showPluginManagerSettings.clicked()
                }
              }

              QfToolButton {
                id: showPluginManagerSettings
                Layout.preferredWidth: 48
                Layout.preferredHeight: 48
                Layout.alignment: Qt.AlignVCenter
                clip: true

                iconSource: Theme.getThemeVectorIcon("ic_ellipsis_black_24dp")
                iconColor: Theme.mainColor
                bgcolor: "transparent"

                onClicked: {
                  pluginManagerSettings.open();
                }
              }
            }

            ListView {
              Layout.fillWidth: true
              Layout.preferredHeight: childrenRect.height
              interactive: false

              model: interfaceSettingsModel

              delegate: listItem
            }

            GridLayout {
              Layout.fillWidth: true
              Layout.leftMargin: 20
              Layout.rightMargin: 20
              Layout.bottomMargin: 5
              Layout.topMargin: 5

              columns: 1
              columnSpacing: 0
              rowSpacing: 5

              visible: platformUtilities.capabilities & PlatformUtilities.AdjustBrightness

              Label {
                Layout.fillWidth: true

                text: qsTr('Dim screen when idling')
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
              }

              QfSlider {
                id: slider
                Layout.fillWidth: true
                value: settings ? settings.value('dimTimeoutSeconds', 60) : 60
                from: 0
                to: 180
                stepSize: 10
                suffixText: " s"
                implicitHeight: 40

                onMoved: function () {
                  iface.setScreenDimmerTimeout(value);
                  settings.setValue('dimTimeoutSeconds', value);
                }
              }

              Label {
                Layout.fillWidth: true
                text: qsTr('Time of inactivity in seconds before the screen brightness get be dimmed to preserve battery.')

                font: Theme.tipFont
                color: Theme.secondaryTextColor
                wrapMode: Text.WordWrap
              }
            }

            GridLayout {
              Layout.fillWidth: true
              Layout.leftMargin: 20
              Layout.rightMargin: 20
              Layout.bottomMargin: 10
              Layout.topMargin: 5

              columns: 1
              columnSpacing: 0
              rowSpacing: 10

              Label {
                Layout.fillWidth: true
                text: qsTr("Appearance:")
                font: Theme.defaultFont
                color: Theme.mainTextColor

                wrapMode: Text.WordWrap
              }

              QfComboBox {
                id: appearanceComboBox
                enabled: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                font: Theme.defaultFont

                popup.font: Theme.defaultFont
                popup.topMargin: mainWindow.sceneTopMargin
                popup.bottomMargin: mainWindow.sceneTopMargin

                model: ListModel {
                  ListElement {
                    name: qsTr('Follow system appearance')
                    value: 'system'
                  }
                  ListElement {
                    name: qsTr('Light theme')
                    value: 'light'
                  }
                  ListElement {
                    name: qsTr('Dark theme')
                    value: 'dark'
                  }
                }
                textRole: "name"
                valueRole: "value"

                property bool initialized: false

                onCurrentValueChanged: {
                  if (initialized) {
                    settings.setValue("appearance", currentValue);
                    Theme.applyAppearance();
                  }
                }

                Component.onCompleted: {
                  var appearance = settings.value("appearance", 'system');
                  currentIndex = indexOfValue(appearance);
                  initialized = true;
                }
              }

              Label {
                Layout.fillWidth: true
                text: qsTr("Font size:")
                font: Theme.defaultFont
                color: Theme.mainTextColor

                wrapMode: Text.WordWrap
              }

              QfComboBox {
                id: fontScaleComboBox
                enabled: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                font: Theme.defaultFont

                popup.font: Theme.defaultFont
                popup.topMargin: mainWindow.sceneTopMargin
                popup.bottomMargin: mainWindow.sceneTopMargin

                model: ListModel {
                  ListElement {
                    name: qsTr('Tiny')
                    value: 0.75
                  }
                  ListElement {
                    name: qsTr('Normal')
                    value: 1.0
                  }
                  ListElement {
                    name: qsTr('Large')
                    value: 1.5
                  }
                  ListElement {
                    name: qsTr('Extra-large')
                    value: 2.0
                  }
                }
                textRole: "name"
                valueRole: "value"

                property bool initialized: false

                onCurrentValueChanged: {
                  if (initialized) {
                    settings.setValue("fontScale", currentValue);
                    Theme.applyFontScale();
                  }
                }

                Component.onCompleted: {
                  var fontScale = settings.value("fontScale", 1.0);
                  currentIndex = indexOfValue(fontScale);
                  initialized = true;
                }
              }

              Label {
                Layout.fillWidth: true
                text: qsTr("Language:")
                font: Theme.defaultFont
                color: Theme.mainTextColor

                wrapMode: Text.WordWrap
              }

              QfComboBox {
                id: languageComboBox
                enabled: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                font: Theme.defaultFont

                popup.font: Theme.defaultFont
                popup.topMargin: mainWindow.sceneTopMargin
                popup.bottomMargin: mainWindow.sceneBottomMargin

                property variant languageCodes: undefined
                property string currentLanguageCode: undefined

                onActivated: {
                  if (currentLanguageCode != undefined) {
                    var newLanguageCode = languageCodes[currentIndex];
                    if (newLanguageCode !== currentLanguageCode) {
                      iface.changeLanguage(newLanguageCode);
                      currentLanguageCode = newLanguageCode;
                    }
                  }
                }

                Component.onCompleted: {
                  var customLanguageCode = settings.value('customLanguage', '');
                  var languages = iface.availableLanguages();
                  languageCodes = [""].concat(Object.keys(languages));
                  var systemLanguage = qsTr("system");
                  var systemLanguageSuffix = systemLanguage !== 'system' ? ' (system)' : '';
                  var items = [systemLanguage + systemLanguageSuffix];
                  languageComboBox.model = items.concat(Object.values(languages));
                  languageComboBox.currentIndex = languageCodes.indexOf(customLanguageCode);
                  currentLanguageCode = customLanguageCode || '';
                }
              }

              Label {
                text: qsTr("Found a missing or incomplete language? %1Join the translator community.%2").arg('<a href="https://explore.transifex.com/opengisch/qfield-for-qgis/">').arg('</a>')
                font: Theme.tipFont
                color: Theme.secondaryTextColor
                textFormat: Qt.RichText
                wrapMode: Text.WordWrap
                Layout.fillWidth: true

                onLinkActivated: link => {
                  Qt.openUrlExternally(link);
                }
              }
            }

            GridLayout {
              Layout.fillWidth: true
              Layout.leftMargin: 20
              Layout.rightMargin: 20

              columns: 2
              columnSpacing: 0
              rowSpacing: 5

              Label {
                text: qsTr('Advanced')
                font: Theme.strongFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.topMargin: 5
                Layout.columnSpan: 2
              }
            }

            ListView {
              Layout.fillWidth: true
              Layout.preferredHeight: childrenRect.height
              interactive: false

              model: advancedSettingsModel

              delegate: listItem
            }

            GridLayout {
              Layout.fillWidth: true
              Layout.leftMargin: 20
              Layout.rightMargin: 20

              columns: 2
              columnSpacing: 0
              rowSpacing: 5

              Label {
                text: qsTr('Network')
                font: Theme.strongFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.topMargin: 10
                Layout.columnSpan: 2
              }

              Label {
                text: qsTr("Enable proxy")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
              }

              QfSwitch {
                id: proxyEnabledSwitch
                Layout.preferredWidth: implicitContentWidth
                Layout.alignment: Qt.AlignTop
                checked: proxySettings ? proxySettings.enabled : false
                onCheckedChanged: {
                  if (proxySettings) {
                    proxySettings.enabled = checked;
                  }
                }
              }

              Label {
                text: qsTr("Proxy type:")
                font: Theme.defaultFont
                color: proxyEnabledSwitch.checked ? Theme.mainTextColor : Theme.secondaryTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.columnSpan: 2
                visible: proxyEnabledSwitch.checked
              }

              QfComboBox {
                id: proxyTypeComboBox
                enabled: proxyEnabledSwitch.checked
                visible: proxyEnabledSwitch.checked
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter
                font: Theme.defaultFont

                popup.font: Theme.defaultFont
                popup.topMargin: mainWindow.sceneTopMargin
                popup.bottomMargin: mainWindow.sceneTopMargin

                model: ListModel {
                  ListElement {
                    name: "HTTP"
                    value: "HttpProxy"
                  }
                  ListElement {
                    name: "SOCKS5"
                    value: "Socks5Proxy"
                  }
                  ListElement {
                    name: qsTr("System Default")
                    value: "DefaultProxy"
                  }
                }
                textRole: "name"
                valueRole: "value"

                property bool initialized: false

                onCurrentValueChanged: {
                  if (initialized && proxySettings) {
                    proxySettings.proxyType = currentValue;
                  }
                }

                Component.onCompleted: {
                  currentIndex = proxySettings ? indexOfValue(proxySettings.proxyType) : 0;
                  if (currentIndex < 0)
                    currentIndex = 0;
                  initialized = true;
                }
              }

              Label {
                text: qsTr("Host:")
                font: Theme.defaultFont
                color: proxyEnabledSwitch.checked ? Theme.mainTextColor : Theme.secondaryTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                visible: proxyEnabledSwitch.checked
              }

              QfTextField {
                id: proxyHostField
                enabled: proxyEnabledSwitch.checked
                visible: proxyEnabledSwitch.checked
                font: Theme.defaultFont
                Layout.fillWidth: true
                placeholderText: qsTr("e.g. proxy.example.com")
                inputMethodHints: Qt.ImhUrlCharactersOnly

                Component.onCompleted: {
                  text = proxySettings ? proxySettings.host : '';
                }

                onTextChanged: {
                  if (proxySettings) {
                    proxySettings.host = text;
                  }
                }
              }

              Label {
                text: qsTr("Port:")
                font: Theme.defaultFont
                color: proxyEnabledSwitch.checked ? Theme.mainTextColor : Theme.secondaryTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                visible: proxyEnabledSwitch.checked
              }

              QfTextField {
                id: proxyPortField
                enabled: proxyEnabledSwitch.checked
                visible: proxyEnabledSwitch.checked
                font: Theme.defaultFont
                Layout.preferredWidth: 100
                horizontalAlignment: TextInput.AlignHCenter
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator {
                  bottom: 0
                  top: 65535
                }

                Component.onCompleted: {
                  text = proxySettings && proxySettings.port > 0 ? proxySettings.port : '';
                }

                onTextChanged: {
                  if (proxySettings) {
                    proxySettings.port = text.length > 0 ? parseInt(text) : 0;
                  }
                }
              }

              Label {
                text: qsTr("Username:")
                font: Theme.defaultFont
                color: proxyEnabledSwitch.checked ? Theme.mainTextColor : Theme.secondaryTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                visible: proxyEnabledSwitch.checked
              }

              QfTextField {
                id: proxyUserField
                enabled: proxyEnabledSwitch.checked
                visible: proxyEnabledSwitch.checked
                font: Theme.defaultFont
                Layout.fillWidth: true
                placeholderText: qsTr("Optional")

                Component.onCompleted: {
                  text = proxySettings ? proxySettings.user : '';
                }

                onTextChanged: {
                  if (proxySettings) {
                    proxySettings.user = text;
                  }
                }
              }

              Label {
                text: qsTr("Password:")
                font: Theme.defaultFont
                color: proxyEnabledSwitch.checked ? Theme.mainTextColor : Theme.secondaryTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                visible: proxyEnabledSwitch.checked
              }

              QfTextField {
                id: proxyPasswordField
                enabled: proxyEnabledSwitch.checked
                visible: proxyEnabledSwitch.checked
                font: Theme.defaultFont
                Layout.fillWidth: true
                echoMode: TextInput.Password
                placeholderText: qsTr("Optional")

                Component.onCompleted: {
                  text = proxySettings ? proxySettings.password : '';
                }

                onTextChanged: {
                  if (proxySettings) {
                    proxySettings.password = text;
                  }
                }
              }

              Label {
                text: qsTr("Excluded URLs (comma-separated):")
                font: Theme.defaultFont
                color: proxyEnabledSwitch.checked ? Theme.mainTextColor : Theme.secondaryTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.columnSpan: 2
                visible: proxyEnabledSwitch.checked
              }

              QfTextField {
                id: proxyExcludedUrlsField
                enabled: proxyEnabledSwitch.checked
                visible: proxyEnabledSwitch.checked
                font: Theme.defaultFont
                Layout.fillWidth: true
                Layout.columnSpan: 2
                placeholderText: qsTr("e.g. localhost, 192.168.*")

                Component.onCompleted: {
                  text = proxySettings ? proxySettings.excludedUrls : '';
                }

                onTextChanged: {
                  if (proxySettings) {
                    proxySettings.excludedUrls = text;
                  }
                }
              }

              Label {
                text: qsTr("Configure a network proxy to route QField's traffic through a proxy server. Useful for corporate networks and VPNs.")
                font: Theme.tipFont
                color: Theme.secondaryTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.columnSpan: 2
              }
            }

            Item {
              // spacer item
              Layout.fillWidth: true
              Layout.fillHeight: true
              Layout.minimumHeight: mainWindow.sceneBottomMargin + 20
            }
          }
        }
      }

      Item {
        ScrollView {
          topPadding: 5
          leftPadding: 20
          rightPadding: 20
          ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
          ScrollBar.vertical: QfScrollBar {}
          contentWidth: positioningGrid.width
          contentHeight: positioningGrid.height
          anchors.fill: parent
          clip: true

          ColumnLayout {
            id: positioningGrid
            width: parent.parent.width
            spacing: 10

            GridLayout {
              Layout.fillWidth: true

              columns: 2
              columnSpacing: 0
              rowSpacing: 5

              Label {
                Layout.fillWidth: true
                Layout.columnSpan: 2
                text: qsTr("Positioning device in use:")
                font: Theme.defaultFont
                color: Theme.mainTextColor

                wrapMode: Text.WordWrap
              }

              RowLayout {
                Layout.fillWidth: true
                Layout.columnSpan: 2

                QfComboBox {
                  id: positioningDeviceComboBox
                  Layout.fillWidth: true
                  Layout.alignment: Qt.AlignVCenter
                  font: Theme.defaultFont

                  popup.font: Theme.defaultFont
                  popup.topMargin: mainWindow.sceneTopMargin
                  popup.bottomMargin: mainWindow.sceneTopMargin

                  textRole: 'DeviceName'
                  valueRole: 'DeviceType'
                  model: PositioningDeviceModel {
                    id: positioningDeviceModel
                  }

                  delegate: ItemDelegate {
                    width: positioningDeviceComboBox.width
                    height: 36
                    icon.source: {
                      switch (DeviceType) {
                      case PositioningDeviceModel.InternalDevice:
                        return Theme.getThemeVectorIcon('ic_internal_receiver_black_24dp');
                      case PositioningDeviceModel.FileDevice:
                        return Theme.getThemeVectorIcon("ic_file_black_24dp");
                      case PositioningDeviceModel.BluetoothDevice:
                        return Theme.getThemeVectorIcon('ic_bluetooth_receiver_black_24dp');
                      case PositioningDeviceModel.TcpDevice:
                        return Theme.getThemeVectorIcon('ic_tcp_receiver_black_24dp');
                      case PositioningDeviceModel.UdpDevice:
                        return Theme.getThemeVectorIcon('ic_udp_receiver_black_24dp');
                      case PositioningDeviceModel.SerialPortDevice:
                        return Theme.getThemeVectorIcon('ic_serial_port_receiver_black_24dp');
                      case PositioningDeviceModel.EgenioussDevice:
                        return Theme.getThemeVectorIcon('ic_egeniouss_receiver_black_24dp');
                      }
                      return '';
                    }
                    icon.width: 24
                    icon.height: 24
                    text: DeviceName
                    font: Theme.defaultFont
                    highlighted: positioningDeviceComboBox.highlightedIndex === index
                  }

                  contentItem: MenuItem {
                    width: positioningDeviceComboBox.width
                    height: 36

                    icon.source: {
                      switch (positioningDeviceComboBox.currentValue) {
                      case PositioningDeviceModel.InternalDevice:
                        return Theme.getThemeVectorIcon('ic_internal_receiver_black_24dp');
                      case PositioningDeviceModel.FileDevice:
                        return Theme.getThemeVectorIcon("ic_file_black_24dp");
                      case PositioningDeviceModel.BluetoothDevice:
                        return Theme.getThemeVectorIcon('ic_bluetooth_receiver_black_24dp');
                      case PositioningDeviceModel.TcpDevice:
                        return Theme.getThemeVectorIcon('ic_tcp_receiver_black_24dp');
                      case PositioningDeviceModel.UdpDevice:
                        return Theme.getThemeVectorIcon('ic_udp_receiver_black_24dp');
                      case PositioningDeviceModel.SerialPortDevice:
                        return Theme.getThemeVectorIcon('ic_serial_port_receiver_black_24dp');
                      case PositioningDeviceModel.EgenioussDevice:
                        return Theme.getThemeVectorIcon('ic_egeniouss_receiver_black_24dp');
                      }
                      return '';
                    }
                    icon.width: 24
                    icon.height: 24

                    text: positioningDeviceComboBox.currentText
                    font: Theme.defaultFont

                    onClicked: positioningDeviceComboBox.popup.open()
                  }

                  Connections {
                    target: positionSource

                    function onDeviceIdChanged() {
                      verticalGridShiftComboBox.reload();
                    }
                  }

                  property bool loaded: false

                  onCurrentIndexChanged: {
                    if (loaded && currentIndex !== -1) {
                      const modelIndex = positioningDeviceModel.index(currentIndex, 0);
                      positioningSettings.positioningDevice = positioningDeviceModel.data(modelIndex, PositioningDeviceModel.DeviceId);
                      positioningSettings.positioningDeviceName = positioningDeviceModel.data(modelIndex, PositioningDeviceModel.DeviceName);
                    }
                  }

                  Component.onCompleted: {
                    currentIndex = positioningDeviceModel.findIndexFromDeviceId(positioningSettings.positioningDevice);
                    loaded = true;
                  }
                }
              }

              RowLayout {
                Layout.fillWidth: true
                Layout.columnSpan: 2

                QfButton {
                  leftPadding: 10
                  rightPadding: 10
                  text: qsTr('Add')

                  onClicked: {
                    positioningDeviceSettings.originalName = '';
                    positioningDeviceSettings.name = '';
                    positioningDeviceSettings.open();
                  }
                }

                Item {
                  Layout.fillWidth: true
                  Layout.alignment: Qt.AlignVCenter
                }

                QfButton {
                  leftPadding: 10
                  rightPadding: 10
                  text: qsTr('Edit')
                  enabled: positioningDeviceComboBox.currentIndex > 0

                  onClicked: {
                    var modelIndex = positioningDeviceModel.index(positioningDeviceComboBox.currentIndex, 0);
                    var name = positioningDeviceModel.data(modelIndex, PositioningDeviceModel.DeviceName);
                    positioningDeviceSettings.originalName = name;
                    positioningDeviceSettings.name = name;
                    positioningDeviceSettings.setType(positioningDeviceModel.data(modelIndex, PositioningDeviceModel.DeviceType));
                    positioningDeviceSettings.setSettings(positioningDeviceModel.data(modelIndex, PositioningDeviceModel.DeviceSettings));
                    positioningDeviceSettings.open();
                  }
                }

                QfButton {
                  leftPadding: 10
                  rightPadding: 10
                  text: qsTr('Remove')
                  enabled: positioningDeviceComboBox.currentIndex > 0

                  onClicked: {
                    var modelIndex = positioningDeviceModel.index(positioningDeviceComboBox.currentIndex, 0);
                    positioningDeviceComboBox.currentIndex = 0;
                    positioningDeviceModel.removeDevice(positioningDeviceModel.data(modelIndex, PositioningDeviceModel.DeviceName));
                  }
                }
              }

              QfButton {
                id: connectButton
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.topMargin: 5
                text: {
                  switch (positionSource.deviceSocketState) {
                  case QAbstractSocket.ConnectedState:
                  case QAbstractSocket.BoundState:
                    return qsTr('Connected to %1').arg(positioningSettings.positioningDeviceName);
                  case QAbstractSocket.UnconnectedState:
                    return qsTr('Connect to %1').arg(positioningSettings.positioningDeviceName);
                  default:
                    return qsTr('Connecting to %1').arg(positioningSettings.positioningDeviceName);
                  }
                }
                enabled: positionSource.deviceSocketState === QAbstractSocket.UnconnectedState
                visible: positionSource.deviceId !== ''

                onClicked: {
                  // make sure positioning is active when connecting to the bluetooth device
                  if (!positioningSettings.positioningActivated) {
                    positioningSettings.positioningActivated = true;
                  } else {
                    positionSource.triggerConnectDevice();
                  }
                }
              }
            }

            GridLayout {
              Layout.fillWidth: true

              columns: 2
              columnSpacing: 0
              rowSpacing: 5

              Label {
                text: qsTr("Show position information")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true

                MouseArea {
                  anchors.fill: parent
                  onClicked: showPositionInformation.toggle()
                }
              }

              QfSwitch {
                id: showPositionInformation
                Layout.preferredWidth: implicitContentWidth
                Layout.alignment: Qt.AlignTop
                checked: positioningSettings.showPositionInformation
                onCheckedChanged: {
                  positioningSettings.showPositionInformation = checked;
                }
              }

              Label {
                id: positionFollowModeLabel
                Layout.fillWidth: true
                Layout.columnSpan: 2
                text: qsTr("Behavior when locked to position:")
                font: Theme.defaultFont
                color: Theme.mainTextColor

                wrapMode: Text.WordWrap
              }

              QfComboBox {
                id: positionFollowModeComboBox
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter
                font: Theme.defaultFont
                model: [qsTr("Follow position only"), qsTr("Follow position and compass orientation"), qsTr("Follow position and movement direction")]

                popup.font: Theme.defaultFont
                popup.topMargin: mainWindow.sceneTopMargin
                popup.bottomMargin: mainWindow.sceneTopMargin

                property bool loaded: false

                Component.onCompleted: {
                  positionFollowModeComboBox.currentIndex = positioningSettings.positionFollowMode;
                  loaded = true;
                }

                onCurrentIndexChanged: {
                  if (loaded) {
                    positioningSettings.positionFollowMode = currentIndex;
                  }
                }
              }

              Label {
                id: positionFollowModeTipLabel
                Layout.fillWidth: true
                text: qsTr("When the map canvas is following or locked to position, it can also rotate to match compass orientation or movement direction.")
                font: Theme.tipFont
                color: Theme.secondaryTextColor

                wrapMode: Text.WordWrap
              }

              Label {
                id: measureLabel
                Layout.fillWidth: true
                Layout.columnSpan: 2
                text: qsTr("Measure (M) value attached to vertices:")
                font: Theme.defaultFont
                color: Theme.mainTextColor

                wrapMode: Text.WordWrap
              }

              QfComboBox {
                id: measureComboBox
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter
                font: Theme.defaultFont

                popup.font: Theme.defaultFont
                popup.topMargin: mainWindow.sceneTopMargin
                popup.bottomMargin: mainWindow.sceneTopMargin

                property bool loaded: false
                Component.onCompleted: {
                  // This list matches the Tracker::MeasureType enum, with SecondsSinceStart removed
                  var measurements = [qsTr("Timestamp (milliseconds since epoch)"), qsTr("Ground speed"), qsTr("Bearing"), qsTr("Horizontal accuracy"), qsTr("Vertical accuracy"), qsTr("PDOP"), qsTr("HDOP"), qsTr("VDOP")];
                  measureComboBox.model = measurements;
                  measureComboBox.currentIndex = positioningSettings.digitizingMeasureType - 1;
                  loaded = true;
                }

                onCurrentIndexChanged: {
                  if (loaded) {
                    positioningSettings.digitizingMeasureType = currentIndex + 1;
                  }
                }
              }

              Label {
                id: measureTipLabel
                Layout.fillWidth: true
                text: qsTr("When digitizing features with the coordinate cursor locked to the current position, the measurement type selected above will be added to the geometry provided it has an M dimension.")
                font: Theme.tipFont
                color: Theme.secondaryTextColor

                wrapMode: Text.WordWrap
              }

              Item {
                // spacer item
                Layout.fillWidth: true
                Layout.fillHeight: true
              }

              Label {
                text: qsTr("Activate accuracy indicator")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true

                MouseArea {
                  anchors.fill: parent
                  onClicked: accuracyIndicator.toggle()
                }
              }

              QfSwitch {
                id: accuracyIndicator
                Layout.preferredWidth: implicitContentWidth
                Layout.alignment: Qt.AlignTop
                checked: positioningSettings.accuracyIndicator
                onCheckedChanged: {
                  positioningSettings.accuracyIndicator = checked;
                }
              }

              Label {
                text: qsTr("Bad accuracy threshold [m]")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                enabled: accuracyIndicator.checked
                visible: accuracyIndicator.checked
                Layout.leftMargin: 8
              }

              QfTextField {
                id: accuracyBadInput
                width: antennaHeightActivated.width
                font: Theme.defaultFont
                enabled: accuracyIndicator.checked
                visible: accuracyIndicator.checked
                horizontalAlignment: TextInput.AlignHCenter
                Layout.preferredWidth: width
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: DoubleValidator {
                  locale: 'C'
                }

                Component.onCompleted: {
                  text = isNaN(positioningSettings.accuracyBad) ? '' : positioningSettings.accuracyBad;
                }

                onTextChanged: {
                  if (text.length === 0 || isNaN(text)) {
                    positioningSettings.accuracyBad = NaN;
                  } else {
                    positioningSettings.accuracyBad = parseFloat(text);
                  }
                }
              }

              Label {
                text: qsTr("Excellent accuracy threshold [m]")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                enabled: accuracyIndicator.checked
                visible: accuracyIndicator.checked
                Layout.leftMargin: 8
              }

              QfTextField {
                id: accuracyExcellentInput
                width: antennaHeightActivated.width
                font: Theme.defaultFont
                enabled: accuracyIndicator.checked
                visible: accuracyIndicator.checked
                horizontalAlignment: TextInput.AlignHCenter
                Layout.preferredWidth: width
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: DoubleValidator {
                  locale: 'C'
                }

                Component.onCompleted: {
                  text = isNaN(positioningSettings.accuracyExcellent) ? '' : positioningSettings.accuracyExcellent;
                }

                onTextChanged: {
                  if (text.length === 0 || isNaN(text)) {
                    positioningSettings.accuracyExcellent = NaN;
                  } else {
                    positioningSettings.accuracyExcellent = parseFloat(text);
                  }
                }
              }

              Label {
                text: qsTr("Enforce accuracy requirement")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                enabled: accuracyIndicator.checked
                visible: accuracyIndicator.checked
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.leftMargin: 8

                MouseArea {
                  anchors.fill: parent
                  onClicked: accuracyIndicator.toggle()
                }
              }

              QfSwitch {
                id: accuracyRequirement
                Layout.preferredWidth: implicitContentWidth
                Layout.alignment: Qt.AlignTop
                enabled: accuracyIndicator.checked
                visible: accuracyIndicator.checked
                checked: positioningSettings.accuracyRequirement
                onCheckedChanged: {
                  positioningSettings.accuracyRequirement = checked;
                }
              }

              Label {
                text: qsTr("When the accuracy indicator is enabled, a badge is attached to the location button and colored <span %1>red</span> if the accuracy value is worse than <i>bad</i>, <span %2>yellow</span> if it falls short of <i>excellent</i>, or <span %3>green</span>.<br><br>In addition, an accuracy restriction mode can be toggled on, which restricts vertex addition when locked to coordinate cursor to positions with an accuracy value worse than the bad threshold.").arg("style='%1'".arg(Theme.toInlineStyles({
                  "color": Theme.accuracyBad
                }))).arg("style='%1'".arg(Theme.toInlineStyles({
                  "color": Theme.accuracyTolerated
                }))).arg("style='%1'".arg(Theme.toInlineStyles({
                  "color": Theme.accuracyExcellent
                })))
                font: Theme.tipFont
                color: Theme.secondaryTextColor
                textFormat: Qt.RichText
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
              }

              Item {
                // empty cell in grid layout
                width: 1
              }

              Label {
                text: qsTr("Enable averaged positioning requirement")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true

                MouseArea {
                  anchors.fill: parent
                  onClicked: averagedPositioning.toggle()
                }
              }

              QfSwitch {
                id: averagedPositioning
                Layout.preferredWidth: implicitContentWidth
                Layout.alignment: Qt.AlignTop
                checked: positioningSettings.averagedPositioning
                onCheckedChanged: {
                  positioningSettings.averagedPositioning = checked;
                }
              }

              Label {
                text: qsTr("Minimum number of positions collected")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                enabled: averagedPositioning.checked
                visible: averagedPositioning.checked
                Layout.leftMargin: 8
              }

              QfTextField {
                id: averagedPositioningMinimumCount
                width: averagedPositioning.width
                font: Theme.defaultFont
                enabled: averagedPositioning.checked
                visible: averagedPositioning.checked
                horizontalAlignment: TextInput.AlignHCenter
                Layout.preferredWidth: width
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: IntValidator {
                  locale: 'C'
                }

                Component.onCompleted: {
                  text = isNaN(positioningSettings.averagedPositioningMinimumCount) ? '' : positioningSettings.averagedPositioningMinimumCount;
                }

                onTextChanged: {
                  if (text.length === 0 || isNaN(text)) {
                    positioningSettings.averagedPositioningMinimumCount = NaN;
                  } else {
                    positioningSettings.averagedPositioningMinimumCount = parseInt(text);
                  }
                }
              }

              Label {
                text: qsTr("Automatically end collection when minimum number is met")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                enabled: averagedPositioning.checked
                visible: averagedPositioning.checked
                Layout.leftMargin: 8

                MouseArea {
                  anchors.fill: parent
                  onClicked: averagedPositioningAutomaticEnd.toggle()
                }
              }

              QfSwitch {
                id: averagedPositioningAutomaticEnd
                Layout.preferredWidth: implicitContentWidth
                Layout.alignment: Qt.AlignTop
                enabled: averagedPositioning.checked
                visible: averagedPositioning.checked
                checked: positioningSettings.averagedPositioningAutomaticStop
                onCheckedChanged: {
                  positioningSettings.averagedPositioningAutomaticStop = checked;
                }
              }

              Label {
                text: qsTr("When enabled, digitizing vertices with a cursor locked to position will only accepted an averaged position from a minimum number of collected positions. Digitizing using averaged positions is done by pressing and holding the add vertex button, which will collect positions until the press is released. Accuracy requirement settings are respected when enabled.")
                font: Theme.tipFont
                color: Theme.secondaryTextColor
                textFormat: Qt.RichText
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
              }

              Item {
                // empty cell in grid layout
                width: 1
              }

              Label {
                text: qsTr("Antenna height compensation")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true

                MouseArea {
                  anchors.fill: parent
                  onClicked: antennaHeightActivated.toggle()
                }
              }

              QfSwitch {
                id: antennaHeightActivated
                Layout.preferredWidth: implicitContentWidth
                Layout.alignment: Qt.AlignTop
                checked: positioningSettings.antennaHeightActivated
                onCheckedChanged: {
                  positioningSettings.antennaHeightActivated = checked;
                }
              }

              Label {
                text: qsTr("Antenna height [m]")
                enabled: antennaHeightActivated.checked
                visible: antennaHeightActivated.checked
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                textFormat: Text.RichText
                Layout.leftMargin: 8
              }

              QfTextField {
                id: antennaHeightInput
                enabled: antennaHeightActivated.checked
                visible: antennaHeightActivated.checked
                width: antennaHeightActivated.width
                font: Theme.defaultFont
                horizontalAlignment: TextInput.AlignHCenter
                Layout.preferredWidth: width
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: DoubleValidator {
                  locale: 'C'
                }

                Component.onCompleted: {
                  text = isNaN(positioningSettings.antennaHeight) ? '' : positioningSettings.antennaHeight;
                }

                onTextChanged: {
                  if (text.length === 0 || isNaN(text)) {
                    positioningSettings.antennaHeight = NaN;
                  } else {
                    positioningSettings.antennaHeight = parseFloat(text);
                  }
                }
              }

              Label {
                text: qsTr("This value will correct the Z values recorded from the positioning device. If a value of 1.6 is entered, the system will automatically subtract 1.6 from each recorded value. Make sure to insert the effective antenna height, i.e. pole length + antenna phase center offset.")
                font: Theme.tipFont
                color: Theme.secondaryTextColor

                wrapMode: Text.WordWrap
                Layout.fillWidth: true
              }

              Item {
                // empty cell in grid layout
                width: 1
              }

              Label {
                text: qsTr("Skip altitude correction")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true

                MouseArea {
                  anchors.fill: parent
                  onClicked: skipAltitudeCorrectionSwitch.toggle()
                }
              }

              QfSwitch {
                id: skipAltitudeCorrectionSwitch
                Layout.preferredWidth: implicitContentWidth
                Layout.alignment: Qt.AlignTop
                checked: positioningSettings.skipAltitudeCorrection
                onCheckedChanged: {
                  positioningSettings.skipAltitudeCorrection = checked;
                }
              }

              Label {
                topPadding: 0
                text: qsTr("Use the altitude as reported by the positioning device. Skip any altitude correction that may be implied by the coordinate system transformation.")
                font: Theme.tipFont
                color: Theme.secondaryTextColor

                wrapMode: Text.WordWrap
                Layout.fillWidth: true
              }

              Item {
                // empty cell in grid layout
                width: 1
              }

              Label {
                text: qsTr("Vertical grid shift in use:")
                font: Theme.defaultFont
                color: Theme.mainTextColor

                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.columnSpan: 2
              }

              QfComboBox {
                id: verticalGridShiftComboBox
                Layout.fillWidth: true
                Layout.columnSpan: 2
                font: Theme.defaultFont

                popup.font: Theme.defaultFont
                popup.topMargin: mainWindow.sceneTopMargin
                popup.bottomMargin: mainWindow.sceneTopMargin

                textRole: "text"
                valueRole: "value"

                model: ListModel {
                  id: verticalGridShiftModel
                }

                onCurrentValueChanged: {
                  if (reloading || currentValue == undefined) {
                    return;
                  }
                  positioningSettings.elevationCorrectionMode = currentValue;
                  if (positioningSettings.elevationCorrectionMode === Positioning.ElevationCorrectionMode.OrthometricFromGeoidFile) {
                    positioningSettings.verticalGrid = currentText;
                  } else {
                    positioningSettings.verticalGrid = "";
                  }
                }

                Component.onCompleted: reload()

                property bool reloading: false
                function reload() {
                  reloading = true;
                  verticalGridShiftComboBox.model.clear();
                  verticalGridShiftComboBox.model.append({
                    "text": qsTr("None"),
                    "value": Positioning.ElevationCorrectionMode.None
                  });
                  if ((positionSource.deviceCapabilities & AbstractGnssReceiver.OrthometricAltitude) != 0) {
                    verticalGridShiftComboBox.model.append({
                      "text": qsTr("Orthometric from device"),
                      "value": Positioning.ElevationCorrectionMode.OrthometricFromDevice
                    });
                  }

                  // Add geoid files to combobox
                  var geoidFiles = platformUtilities.availableGrids();
                  for (var i = 0; i < geoidFiles.length; i++)
                    verticalGridShiftComboBox.model.append({
                      "text": geoidFiles[i],
                      "value": Positioning.ElevationCorrectionMode.OrthometricFromGeoidFile
                    });
                  if (positioningSettings.elevationCorrectionMode === Positioning.ElevationCorrectionMode.None) {
                    verticalGridShiftComboBox.currentIndex = indexOfValue(positioningSettings.elevationCorrectionMode);
                    positioningSettings.verticalGrid = "";
                  } else if (positioningSettings.elevationCorrectionMode === Positioning.ElevationCorrectionMode.OrthometricFromDevice) {
                    if ((positionSource.deviceCapabilities & AbstractGnssReceiver.OrthometricAltitude) != 0)
                      verticalGridShiftComboBox.currentIndex = verticalGridShiftComboBox.indexOfValue(positioningSettings.elevationCorrectionMode);
                    else
                      // Orthometric not available -> fallback to None
                      verticalGridShiftComboBox.currentIndex = verticalGridShiftComboBox.indexOfValue(Positioning.ElevationCorrectionMode.None);
                    positioningSettings.verticalGrid = "";
                  } else if (positioningSettings.elevationCorrectionMode === Positioning.ElevationCorrectionMode.OrthometricFromGeoidFile) {
                    var currentVerticalGridFileIndex = verticalGridShiftComboBox.find(positioningSettings.verticalGrid);
                    if (currentVerticalGridFileIndex < 1)
                      // Vertical index file not found -> fallback to None
                      verticalGridShiftComboBox.currentIndex = verticalGridShiftComboBox.indexOfValue(Positioning.ElevationCorrectionMode.None);
                    else
                      verticalGridShiftComboBox.currentIndex = currentVerticalGridFileIndex;
                  } else {
                    console.log("Warning unknown elevationCorrectionMode: '%1'".arg(positioningSettings.elevationCorrectionMode));

                    // Unknown mode -> fallback to None
                    verticalGridShiftComboBox.currentIndex = verticalGridShiftComboBox.indexOfValue(Positioning.ElevationCorrectionMode.None);
                  }
                  reloading = false;
                }
              }

              Label {
                topPadding: 0
                rightPadding: antennaHeightActivated.width
                text: qsTr("Vertical grid shift is used to increase the altitude accuracy.")
                font: Theme.tipFont
                color: Theme.secondaryTextColor

                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.columnSpan: 2
              }

              Label {
                text: qsTr("Log NMEA sentences from device to file")
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                visible: positionSource.deviceCapabilities & AbstractGnssReceiver.Logging

                MouseArea {
                  anchors.fill: parent
                  onClicked: positionLogging.toggle()
                }
              }

              QfSwitch {
                id: positionLogging
                Layout.preferredWidth: implicitContentWidth
                Layout.alignment: Qt.AlignTop
                visible: positionSource.deviceCapabilities & AbstractGnssReceiver.Logging
                checked: positioningSettings.logging
                onCheckedChanged: {
                  positioningSettings.logging = checked;
                }
              }
            }

            Item {
              // spacer item
              Layout.fillWidth: true
              Layout.fillHeight: true
              Layout.minimumHeight: mainWindow.sceneBottomMargin + 20
            }
          }
        }
      }

      Item {
        VariableEditor {
          id: variableEditor
          anchors.fill: parent
          anchors.margins: 4
          anchors.bottomMargin: 4 + mainWindow.sceneBottomMargin
        }
      }
    }
  }

  PositioningDeviceSettings {
    id: positioningDeviceSettings

    property string originalName: ''

    onApply: {
      if (originalName != '') {
        positioningDeviceModel.removeDevice(originalName);
      }
      var name = positioningDeviceSettings.name;
      var type = positioningDeviceSettings.type;
      var settings = positioningDeviceSettings.getSettings();
      if (name === '') {
        name = positioningDeviceSettings.generateName();
      }
      var index = positioningDeviceModel.addDevice(type, name, settings);
      positioningDeviceComboBox.currentIndex = index;
    }
  }

  header: QfPageHeader {
    title: qsTr("%1 Settings").arg(appName)

    showBackButton: true
    showApplyButton: false
    showCancelButton: false

    topMargin: mainWindow.sceneTopMargin

    onFinished: {
      parent.finished();
      variableEditor.apply();
    }
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true;
      variableEditor.apply();
      finished();
    }
  }
}
