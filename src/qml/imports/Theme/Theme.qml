pragma Singleton

import QtQuick 2.12

QtObject {
    readonly property color darkRed: "#900000"
    readonly property color darkGray: "#212121"
    readonly property color darkGraySemiOpaque: "#88212121"
    readonly property color gray: "#888888"
    readonly property color lightGray: "#dddddd"
    readonly property color light: "#ffffff"
    readonly property color hyperlinkBlue: '#0000EE'

    property color mainColor: "#80cc28"
    property color errorColor: "#c0392b"
    property color warningColor: "orange"
    property color cloudColor: "#4c6dac"

    property font defaultFont
    defaultFont.pointSize: 16

    property font tipFont
    tipFont.pointSize: 14

    property font resultFont
    resultFont.pointSize: 13

    property font strongFont
    strongFont.pointSize: defaultFont.pointSize
    strongFont.bold: true

    property font strongTipFont
    strongTipFont.pointSize: tipFont.pointSize
    strongTipFont.bold: true

    property font secondaryTitleFont
    secondaryTitleFont.pointSize: 18

    property font titleFont
    titleFont.pointSize: 20

    function getThemeIcon(name) {
      var ppiName
      if ( ppi >= 360 )
        ppiName = "xxxhdpi";
       else if ( ppi >= 270 )
        ppiName = "xxhdpi";
       else if ( ppi >= 180 )
        ppiName = "xhdpi";
       else if ( ppi >= 135 )
        ppiName = "hdpi";
       else
        ppiName = "mdpi";

      var theme = 'qfield';

      var path = '/themes/' + theme + '/' + ppiName + '/' + name + '.png';
      return path;
    }

    function getThemeVectorIcon(name) {
      var theme = 'qfield';

      var path = '/themes/' + theme + '/nodpi/' + name + '.svg';
      return path;
    }

    function colorToHtml(color) {
      return "rgba(%1,%2,%3,%4)".arg(Math.floor(Theme.errorColor.r * 255)).arg(Math.floor(Theme.errorColor.g * 255)).arg(Math.floor(Theme.errorColor.b * 255)).arg(Math.floor(Theme.errorColor.a * 255));
      
    }

    function toInlineStyles(styleProperties) {
      var styles = ''

      for (var property in styleProperties) {
        var value = styleProperties[property];
        styles += property
        styles += ': '
        styles += typeof value == 'color'
          ? colorToHtml(value)
          : value
        styles += ';'
      }

      return styles;
    }
}

