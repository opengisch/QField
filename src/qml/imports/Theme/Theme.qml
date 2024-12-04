pragma Singleton
import QtQuick
import QtQuick.Controls.Material

QtObject {
  property bool darkTheme: false

  property color mainColor: "#80cc28"
  property color mainOverlayColor: "#ffffff"

  property color mainBackgroundColor: darkTheme ? "#303030" : "#fafafa"
  property color mainBackgroundColorSemiOpaque: darkTheme ? "#bb303030" : "#bbfafafa"

  property color mainTextColor: darkTheme ? "#EEEEEE" : "#000000"
  property color mainTextDisabledColor: darkTheme ? "#73EEEEEE" : "#73000000"
  property color secondaryTextColor: darkTheme ? "#AAAAAA" : "#999999"

  property color controlBackgroundColor: darkTheme ? "#202020" : "#ffffff"
  property color controlBackgroundAlternateColor: darkTheme ? "#202020" : "#e6e6e6" // used by feature form editor widgets
  property color controlBackgroundDisabledColor: "#33555555"
  property color controlBorderColor: darkTheme ? "#404040" : "#e6e6e6"

  property color buttonTextColor: darkTheme ? "#202020" : "#ffffff"

  property color toolButtonColor: "#ffffff"
  property color toolButtonBackgroundColor: darkGray
  property color toolButtonBackgroundSemiOpaqueColor: darkGraySemiOpaque

  property color darkRed: "#c0392b"
  property color darkGray: "#212121"
  property color darkGraySemiOpaque: "#4D212121"
  property color gray: "#888888"
  property color lightGray: "#dddddd"
  property color lightestGray: "#eeeeee"
  property color light: "#ffffff"
  property color hyperlinkBlue: '#0000EE'

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

  property color vertexColor: "#FF0000"
  property color vertexColorSemiOpaque: "#40FF0000"
  property color vertexSelectedColor: "#0000FF"
  property color vertexSelectedColorSemiOpaque: "#200000FF"
  property color vertexNewColor: "#4CAF50"
  property color vertexNewColorSemiOpaque: "#404CAF50"

  property color processingPreview: '#99000000'
  property color scrollBarBackgroundColor: darkTheme ? mainBackgroundColorSemiOpaque : "#aaffffff"

  property color accentColor: '#4CAF50'
  property color accentLightColor: '#994CAF50'

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

  function applyAppearance() {
    var appearance = settings ? settings.value('appearance', 'system') : undefined;
    if (appearance === undefined || appearance === 'system') {
      darkTheme = platformUtilities.isSystemDarkTheme();
    } else if (appearance === 'light') {
      darkTheme = false;
    } else if (appearance === 'dark') {
      darkTheme = true;
    }
    Material.theme = darkTheme ? "Dark" : "Light";
    mainBackgroundColor = Material.backgroundColor;
    mainTextColor = Material.foreground;
  }

  function applyFontScale() {
    fontScale = settings ? settings.value('fontScale', 1.0) : 1.0;
  }

  Component.onCompleted: {
    applyAppearance();
    applyFontScale();
  }
}
