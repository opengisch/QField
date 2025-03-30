pragma Singleton
import QtQuick
import QtQuick.Controls.Material

QtObject {
  id: object

  property var darkThemeColors: {
    "mainColor": "#80cc28",
    "mainOverlayColor": "#ffffff",
    "accentColor": "#4CAF50",
    "accentLightColor": "#994CAF50",
    "mainBackgroundColor": "#303030",
    "mainBackgroundColorSemiOpaque": "#bb303030",
    "mainTextColor": "#EEEEEE",
    "mainTextDisabledColor": "#73EEEEEE",
    "secondaryTextColor": "#AAAAAA",
    "controlBackgroundColor": "#202020",
    "controlBackgroundAlternateColor": "#202020",
    "controlBackgroundDisabledColor": "#33555555",
    "controlBorderColor": "#404040",
    "buttonTextColor": "#202020",
    "toolButtonColor": "#ffffff",
    "toolButtonBackgroundColor": Theme.darkGray,
    "toolButtonBackgroundSemiOpaqueColor": Theme.darkGraySemiOpaque,
    "scrollBarBackgroundColor": "#bb303030"
  }

  property var lightThemeColors: {
    "mainColor": "#80cc28",
    "mainOverlayColor": "#ffffff",
    "accentColor": "#4CAF50",
    "accentLightColor": "#994CAF50",
    "mainBackgroundColor": "#fafafa",
    "mainBackgroundColorSemiOpaque": "#bbfafafa",
    "mainTextColor": "#000000",
    "mainTextDisabledColor": "#73000000",
    "secondaryTextColor": "#999999",
    "controlBackgroundColor": "#ffffff",
    "controlBackgroundAlternateColor": "#e6e6e6",
    "controlBackgroundDisabledColor": "#33555555",
    "controlBorderColor": "#e6e6e6",
    "buttonTextColor": "#ffffff",
    "toolButtonColor": "#ffffff",
    "toolButtonBackgroundColor": Theme.darkGray,
    "toolButtonBackgroundSemiOpaqueColor": Theme.darkGraySemiOpaque,
    "scrollBarBackgroundColor": "#aaffffff"
  }

  property bool darkTheme: false

  property color mainColor: "#80cc28"
  property color mainOverlayColor: "#ffffff"
  property color accentColor: "#4CAF50"
  property color accentLightColor: "#994CAF50"

  property color mainBackgroundColor: "#fafafa"
  property color mainBackgroundColorSemiOpaque: "#bbfafafa"
  property color mainTextColor: "#000000"
  property color mainTextDisabledColor: "#73000000"

  property color secondaryTextColor: "#999999"

  property color controlBackgroundColor: "#ffffff"
  property color controlBackgroundAlternateColor: "#e6e6e6"
  property color controlBackgroundDisabledColor: "#33555555"
  property color controlBorderColor: "#e6e6e6"

  property color buttonTextColor: "#ffffff"

  property color toolButtonColor: "#ffffff"
  property color toolButtonBackgroundColor: Theme.darkGray
  property color toolButtonBackgroundSemiOpaqueColor: Theme.darkGraySemiOpaque

  property color scrollBarBackgroundColor: "#aaffffff"

  property color darkRed: "#c0392b"
  property color darkGray: "#212121"
  property color darkGraySemiOpaque: "#4D212121"
  property color gray: "#888888"
  property color lightGray: "#dddddd"
  property color lightestGray: "#eeeeee"
  property color light: "#ffffff"

  property color errorColor: darkTheme ? "#df3422" : "#c0392b"
  property color warningColor: "orange"
  property color cloudColor: "#4c6dac"

  property color positionColor: "#64b5f6"
  property color positionColorSemiOpaque: "#3364b5f6"
  property color positionBackgroundColor: "#3364b5f6"
  property color darkPositionColor: "#2374b5"
  property color darkPositionColorSemiOpaque: "#882374b5"

  property color accuracyBad: "#c0392b"
  property color accuracyTolerated: "orange"
  property color accuracyExcellent: "#80cc28"

  property color navigationColor: "#984ea3"
  property color navigationColorSemiOpaque: "#99984ea3"
  property color navigationBackgroundColor: "#33984ea3"

  property color sensorBackgroundColor: "#33999999"

  property color bookmarkDefault: "#80cc28"
  property color bookmarkOrange: "orange"
  property color bookmarkRed: "#c0392b"
  property color bookmarkBlue: "#64b5f6"
  property color qfieldcloudBlue: "#4a6fae"

  property color vertexColor: "#FF0000"
  property color vertexColorSemiOpaque: "#40FF0000"
  property color vertexSelectedColor: "#0000FF"
  property color vertexSelectedColorSemiOpaque: "#200000FF"
  property color vertexNewColor: "#4CAF50"
  property color vertexNewColorSemiOpaque: "#404CAF50"

  property color processingPreview: '#99000000'

  property real fontScale: 1.0

  property font defaultFont: Qt.font({
      "pointSize": systemFontPointSize * fontScale,
      "weight": Font.Normal
    })
  property font tinyFont: Qt.font({
      "pointSize": systemFontPointSize * fontScale * 0.75,
      "weight": Font.Normal
    })
  property font tipFont: Qt.font({
      "pointSize": systemFontPointSize * fontScale * 0.875,
      "weight": Font.Normal
    })
  property font resultFont: Qt.font({
      "pointSize": systemFontPointSize * fontScale * 0.8125,
      "weight": Font.Normal
    })
  property font strongFont: Qt.font({
      "pointSize": systemFontPointSize * fontScale,
      "bold": true,
      "weight": Font.Bold
    })
  property font strongTipFont: Qt.font({
      "pointSize": systemFontPointSize * fontScale * 0.875,
      "bold": true,
      "weight": Font.Bold
    })
  property font secondaryTitleFont: Qt.font({
      "pointSize": systemFontPointSize * fontScale * 1.125,
      "weight": Font.Normal
    })
  property font titleFont: Qt.font({
      "pointSize": systemFontPointSize * fontScale * 1.25,
      "weight": Font.Normal
    })

  readonly property int popupScreenEdgeMargin: 40

  readonly property int menuItemIconlessLeftPadding: 52
  readonly property int menuItemLeftPadding: 12
  readonly property int menuItemCheckLeftPadding: 16

  function getThemeIcon(name) {
    var ppiName;
    if (ppi >= 360)
      ppiName = "xxxhdpi";
    else if (ppi >= 270)
      ppiName = "xxhdpi";
    else if (ppi >= 180)
      ppiName = "xhdpi";
    else if (ppi >= 135)
      ppiName = "hdpi";
    else
      ppiName = "mdpi";
    var theme = 'qfield';
    var path = 'qrc:/themes/' + theme + '/' + ppiName + '/' + name + '.png';
    return path;
  }

  function getThemeVectorIcon(name) {
    var theme = 'qfield';
    var path = 'qrc:/themes/' + theme + '/nodpi/' + name + '.svg';
    return path;
  }

  function colorToHtml(color) {
    return "rgba(%1,%2,%3,%4)".arg(Math.floor(Theme.errorColor.r * 255)).arg(Math.floor(Theme.errorColor.g * 255)).arg(Math.floor(Theme.errorColor.b * 255)).arg(Math.floor(Theme.errorColor.a * 255));
  }

  function toInlineStyles(styleProperties) {
    var styles = '';
    for (var property in styleProperties) {
      var value = styleProperties[property];
      styles += property;
      styles += ': ';
      styles += typeof value == 'color' ? colorToHtml(value) : value;
      styles += ';';
    }
    return styles;
  }

  function applyColors(colors) {
    const names = Object.keys(colors);
    for (const name of names) {
      if (object.hasOwnProperty(name)) {
        object[name] = colors[name];
      }
    }
  }

  function applyAppearance(colors, baseAppearance) {
    const appearance = baseAppearance !== undefined ? baseAppearance : settings ? settings.value('appearance', 'system') : undefined;
    if (appearance === undefined || appearance === 'system') {
      darkTheme = platformUtilities.isSystemDarkTheme();
    } else {
      darkTheme = appearance === 'dark';
    }
    Material.theme = darkTheme ? "Dark" : "Light";
    applyColors(darkTheme ? Theme.darkThemeColors : Theme.lightThemeColors);
    mainBackgroundColor = Material.backgroundColor;
    mainTextColor = Material.foreground;
    if (colors !== undefined) {
      applyColors(colors);
    }
  }

  function applyFontScale() {
    fontScale = settings ? settings.value('fontScale', 1.0) : 1.0;
  }

  Component.onCompleted: {
    applyAppearance();
    applyFontScale();
  }
}
