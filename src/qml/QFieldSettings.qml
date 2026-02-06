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
      //title: qsTr("Show zoom controls")
      title: qsTr("확대/축소 버튼 표시")
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
      //title: qsTr("Enable map rotation")
      title: qsTr("지도 회전 사용")
      //description: qsTr("When switched on, the map can be rotated by the user.")
      description: qsTr("이 설정을 켜면 사용자가 지도를 회전할 수 있습니다.")
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
      //title: qsTr("Use volume keys to digitize")
      title: qsTr("볼륨 키로 지도 편집")
      //description: qsTr("If enabled, pressing the device's volume up key will add a vertex while pressing volume down key will remove the last entered vertex during digitizing sessions.")
      description: qsTr("이 설정을 켜면 지도 편집 중 볼륨 올림 키를 눌러 꼭짓점을 추가하고, 볼륨 내림 키를 눌러 마지막으로 입력한 꼭짓점을 삭제할 수 있습니다.")
      settingAlias: "digitizingVolumeKeys"
      isVisible: true
    }
    ListElement {
      //title: qsTr("Allow finger tap on canvas to add vertices")
      title: qsTr("지도 화면 터치로 꼭짓점 추가 허용")
      //description: qsTr("When enabled, tapping on the map canvas with a finger will add a vertex at the tapped location.")
      description: qsTr("이 설정을 켜면 지도 화면을 손가락으로 터치했을 때 해당 위치에 꼭짓점이 추가됩니다.")
      settingAlias: "fingerTapDigitizing"
      isVisible: true
    }
    ListElement {
      title: qsTr("Consider mouse as a touchscreen device")
      //description: qsTr("When enabled, the mouse will act as if it was a finger. When disabled, the mouse will match the stylus behavior.")
      description: qsTr("이 설정을 켜면 마우스가 손가락처럼 동작합니다. 끄면 마우스가 스타일러스(펜) 방식으로 동작합니다.")
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
      //title: qsTr("Maximize feature form")
      title: qsTr("항목 입력 폼 최대화")
      description: ''
      settingAlias: "fullScreenIdentifyView"
      isVisible: true
    }
    ListElement {
      //title: qsTr("Open feature form for single feature identification")
      title: qsTr("단일 항목 선택 시 입력 폼 자동 열기")
      //description: qsTr("When enabled, the feature form will open automatically if only one feature is identified, skipping the feature list.")
      description: qsTr("이 설정을 켜면 하나의 항목만 식별될 경우 항목 목록을 건너뛰고 입력 폼이 자동으로 열립니다.")
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
      //title: qsTr("Auto-zoom to identified feature(s)")
      title: qsTr("식별된 항목으로 자동 확대")
      //description: qsTr("When enabled, the map will automatically zoom to show all identified features, as well as the individual selected feature when the feature form is opened.")
      description: qsTr("이 설정을 켜면 식별된 모든 항목이 보이도록 지도가 자동으로 확대되며, 항목 입력 폼을 열 때 선택된 개별 항목으로도 자동 확대됩니다.")
      settingAlias: "autoZoomToIdentifiedFeature"
      isVisible: true
    }
  }

  ListModel {
    id: advancedSettingsModel
    ListElement {
      //title: qsTr("Render preview content around visible map canvas")
      title: qsTr("지도 화면 주변 미리보기 렌더링")
      //description: qsTr("If enabled, areas just outside of the visible map canvas extent will be partially rendered to allow preview when zooming and panning.")
      description: qsTr("이 설정을 켜면 현재 보이는 지도 화면 범위의 바깥 영역도 일부 렌더링되어 확대하거나 이동할 때 미리보기가 가능합니다.")
      settingAlias: "previewJobsEnabled"
      isVisible: true
    }
    ListElement {
      //title: qsTr("Enable auto-save mode")
      title: qsTr("자동 저장 모드 사용")
      //description: qsTr("If enabled, newly-added features are stored as soon as it has having a valid geometry and the constraints are fulfilled and edited atributes are commited immediately.")
      description: qsTr("이 설정을 켜면 새로 추가된 항목이 유효한 도형을 갖추고 제약 조건을 충족하는 즉시 저장되며, 수정된 속성도 바로 반영됩니다.")
      settingAlias: "autoSave"
      isVisible: true
    }
    ListElement {
      //title: qsTr("Use native camera")
      title: qsTr("기기 기본 카메라 사용")
      //description: qsTr("If disabled, QField will use a minimalist internal camera instead of the camera app on the device.<br>Tip: Enable this option and install the open camera app to create geo tagged photos.")
      description: qsTr("이 설정을 끄면 QField는 기기에 설치된 카메라 앱 대신 간단한 내부 카메라를 사용합니다.<br>팁: 이 옵션을 켜고 Open Camera 앱을 설치하면 위치 정보가 포함된 사진을 촬영할 수 있습니다.")
      settingAlias: "nativeCamera2"
      isVisible: true
    }
    ListElement {
      title: qsTr("Send anonymized metrics")
      description: qsTr("If enabled, anonymized metrics will be collected and sent to help improve QField for everyone.")
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
                //text: qsTr('Map Canvas')
                text: qsTr('지도 화면')
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
                //text: qsTr("Map canvas rendering quality:")
                text: qsTr("지도 화면 렌더링 품질:")
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
                    name: qsTr("최고 품질")
                    value: 1.0
                  }
                  ListElement {
                    name: qsTr("낮은 품질")
                    value: 0.75
                  }
                  ListElement {
                    name: qsTr("최저 품질")
                    value: 0.5
                  }
                  //ListElement {
                  //  name: qsTr('Best quality')
                  //  value: 1.0
                  //}
                  //ListElement {
                  //  name: qsTr('Lower quality')
                  //  value: 0.75
                  //}
                  //ListElement {
                  //  name: qsTr('Lowest quality')
                  //  value: 0.5
                  //}
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
                //text: qsTr("A lower quality trades rendering precision in favor of lower memory usage and rendering time.")
                text: qsTr("낮은 품질은 렌더링 정확도를 낮추는 대신 메모리 사용량과 렌더링 시간을 줄입니다.")
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
                //text: qsTr('Digitizing & Editing')
                text: qsTr('지도 편집')
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
                //text: qsTr('User Interface')
                text: qsTr('사용자 인터페이스')
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
                //text: qsTr("Manage plugins")
                text: qsTr("플러그인 관리")
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
                //text: qsTr('Time of inactivity in seconds before the screen brightness get be dimmed to preserve battery.')
                text: qsTr('배터리 절약을 위해 화면 밝기가 낮아지기까지의 비활성 시간(초)입니다.')

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
                //text: qsTr("Appearance:")
                text: qsTr("화면 모드:")
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
                    name: qsTr("시스템 설정 따르기")
                    value: "system"
                  }
                  ListElement {
                    name: qsTr("라이트")
                    value: "light"
                  }
                  ListElement {
                    name: qsTr("다크")
                    value: "dark"
                  }
                  //ListElement {
                  //  name: qsTr('Follow system appearance')
                  //  value: 'system'
                  //}
                  //ListElement {
                  //  name: qsTr('Light theme')
                  //  value: 'light'
                  //}
                  //ListElement {
                  //  name: qsTr('Dark theme')
                  //  value: 'dark'
                  //}
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
                //text: qsTr("Font size:")
                text: qsTr("글자 크기:")
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
                    name: qsTr("아주 작게")
                    value: 0.75
                  }
                  ListElement {
                    name: qsTr("보통")
                    value: 1.0
                  }
                  ListElement {
                    name: qsTr("크게")
                    value: 1.5
                  }
                  ListElement {
                    name: qsTr("아주 크게")
                    value: 2.0
                  }
                  //ListElement {
                  //  name: qsTr('Tiny')
                  //  value: 0.75
                  //}
                  //ListElement {
                  //  name: qsTr('Normal')
                  //  value: 1.0
                  //}
                  //ListElement {
                  //  name: qsTr('Large')
                  //  value: 1.5
                  //}
                  //ListElement {
                  //  name: qsTr('Extra-large')
                  //  value: 2.0
                  //}
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
                //text: qsTr("Language:")
                text: qsTr("언어:")
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

                onCurrentIndexChanged: {
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
                //text: qsTr('Advanced')
                text: qsTr('고급 설정')
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

                  onCurrentIndexChanged: {
                    var modelIndex = positioningDeviceModel.index(currentIndex, 0);
                    positioningSettings.positioningDevice = positioningDeviceModel.data(modelIndex, PositioningDeviceModel.DeviceId);
                    positioningSettings.positioningDeviceName = positioningDeviceModel.data(modelIndex, PositioningDeviceModel.DeviceName);
                  }

                  Component.onCompleted: {
                    currentIndex = positioningDeviceModel.findIndexFromDeviceId(settings.value('positioningDevice', ''));
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
                id: measureLabel
                Layout.fillWidth: true
                Layout.columnSpan: 2
                //text: qsTr("Measure (M) value attached to vertices:")
                text: qsTr("꼭짓점에 연결된 M 값 측정:")
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
                //text: qsTr("When digitizing features with the coordinate cursor locked to the current position, the measurement type selected above will be added to the geometry provided it has an M dimension.")
                text: qsTr("현재 위치에 좌표 커서를 고정한 상태로 객체를 편집할 때, 도형에 M 차원이 있는 경우 위에서 선택한 측정 유형이 도형에 추가됩니다.")
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
                //text: qsTr("Activate accuracy indicator")
                text: qsTr("정확도 표시 활성화")
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
                //text: qsTr("When the accuracy indicator is enabled, a badge is attached to the location button and colored <span %1>red</span> if the accuracy value is worse than <i>bad</i>, <span %2>yellow</span> if it falls short of <i>excellent</i>, or <span %3>green</span>.<br><br>In addition, an accuracy restriction mode can be toggled on, which restricts vertex addition when locked to coordinate cursor to positions with an accuracy value worse than the bad threshold.").arg("style='%1'".arg(Theme.toInlineStyles({
                text: qsTr("정확도 표시를 활성화하면 위치 버튼에 배지가 표시되며, 정확도 값이 <i>나쁨</i> 기준보다 낮으면 <span %1>red</span>, <i>우수</i> 기준에 미치지 못하면 <span %2>yellow</span>, 기준을 충족하면 <span %3>green</span>으로 표시됩니다.<br><br>또한 정확도 제한 모드를 켤 수 있으며, 이 모드를 활성화하면 좌표 커서를 고정한 상태에서 꼭짓점 추가가 <i>나쁨</i> 기준보다 정확도가 더 나쁜 위치로 제한됩니다.").arg("style='%1'".arg(Theme.toInlineStyles({
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
                //text: qsTr("Enable averaged positioning requirement")
                text: qsTr("평균 위치 정확도 조건 사용")
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
                //text: qsTr("When enabled, digitizing vertices with a cursor locked to position will only accepted an averaged position from a minimum number of collected positions. Digitizing using averaged positions is done by pressing and holding the add vertex button, which will collect positions until the press is released. Accuracy requirement settings are respected when enabled.")
                text: qsTr("이 설정을 켜면 위치에 고정된 커서로 꼭짓점을 편집할 때, 수집된 최소 개수 이상의 위치를 평균한 값만 허용됩니다. 평균 위치를 사용한 편집은 꼭짓점 추가 버튼을 길게 눌러 수행하며, 버튼을 누르는 동안 위치가 수집되고 손을 떼면 평균 위치가 적용됩니다. 정확도 요구 조건이 활성화된 경우 해당 설정도 함께 적용됩니다.")
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
                //text: qsTr("Antenna height compensation")
                text: qsTr("안테나 높이 보정")
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
                //text: qsTr("This value will correct the Z values recorded from the positioning device. If a value of 1.6 is entered, QField will automatically subtract 1.6 from each recorded value. Make sure to insert the effective antenna height, i.e. pole length + antenna phase centre offset.")
                text: qsTr("이 값은 위치 장치에서 기록된 Z 값을 보정합니다. 예를 들어 1.6을 입력하면 QField는 기록된 각 값에서 1.6을 자동으로 차감합니다. 폴 길이와 안테나 위상 중심 오프셋을 합산한 실제 안테나 높이를 입력하세요.")

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
                //text: qsTr("Skip altitude correction")
                text: qsTr("고도 보정 건너뛰기")
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
                //text: qsTr("Use the altitude as reported by the positioning device. Skip any altitude correction that may be implied by the coordinate system transformation.")
                text: qsTr("위치 장치에서 제공하는 고도 값을 그대로 사용합니다. 좌표계 변환으로 인해 적용될 수 있는 모든 고도 보정은 수행하지 않습니다.")

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
                //text: qsTr("Vertical grid shift in use:")
                text: qsTr("사용 중인 수직 격자 변위:")
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
                //text: qsTr("Vertical grid shift is used to increase the altitude accuracy.")
                text: qsTr("수직 격자 변위는 고도 정확도를 향상시키기 위해 사용됩니다.")
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
      positioningDeviceComboBox.onCurrentIndexChanged();
    }
  }

  header: QfPageHeader {
    title: qsTr("QField Settings")

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
