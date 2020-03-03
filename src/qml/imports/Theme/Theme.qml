pragma Singleton

import QtQuick 2.12

QtObject {
    readonly property color darkGray: "#212121"
    readonly property color lightGray: "#dddddd"
    readonly property color light: "#ffffff"

    property color mainColor: "#80cc28"

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
      var ppi = dp / 0.00768443;

      var ppiRange
      if ( ppi >= 360 )
        ppi = "xxxhdpi";
       else if ( ppi >= 270 )
        ppi = "xxhdpi";
       else if ( ppi >= 180 )
        ppi = "xhdpi";
       else if ( ppi >= 135 )
        ppi = "hdpi";
       else
        ppi = "mdpi";

      var theme = 'qfield';

      var path = '/themes/' + theme + '/' + ppi + '/' + name + '.png';
      return path;
    }

    function getThemeVectorIcon(name) {
      var theme = 'qfield';

      var path = '/themes/' + theme + '/nodpi/' + name + '.svg';
      return path;
    }
}

