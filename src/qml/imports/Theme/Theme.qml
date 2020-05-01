pragma Singleton

import QtQuick 2.12

QtObject {
    readonly property color darkRed: "#900000"
    readonly property color darkGray: "#212121"
    readonly property color darkGraySemiOpaque: "#88212121"
    readonly property color gray: "#888888"
    readonly property color lightGray: "#dddddd"
    readonly property color light: "#ffffff"

    property color mainColor: "#80cc28"
    property color errorColor: "#c0392b"
    property color warningColor: "orange"

    property font defaultFont
    defaultFont.pointSize: 16

    property font tipFont
    tipFont.pointSize: 14

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
}

