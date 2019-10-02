pragma Singleton

import QtQuick 2.12

QtObject {
    readonly property color darkGray: "#212121"
    readonly property color lightGray: "#dddddd"
    readonly property color light: "#ffffff"

    property color mainColor: "#80cc28"

    property int defaultFontSizePt: 14

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
}

