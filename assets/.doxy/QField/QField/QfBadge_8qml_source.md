

# File QfBadge.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfBadge.qml**](QfBadge_8qml.md)

[Go to the documentation of this file](QfBadge_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield

Rectangle {
  id: badge

  enum Alignment {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
  }

  property alias badgeText: inText
  property bool enableGradient: false
  property int alignment: QfBadge.Alignment.TopRight

  property int topMargin: 2
  property int bottomMargin: 2
  property int leftMargin: 2
  property int rightMargin: 2

  width: 12
  height: width
  radius: width / 2

  border.width: 1.5
  border.color: Theme.mainBackgroundColor

  anchors {
    top: (alignment === QfBadge.Alignment.TopLeft || alignment === QfBadge.Alignment.TopRight) ? parent.top : undefined
    bottom: (alignment === QfBadge.Alignment.BottomLeft || alignment === QfBadge.Alignment.BottomRight) ? parent.bottom : undefined
    left: (alignment === QfBadge.Alignment.TopLeft || alignment === QfBadge.Alignment.BottomLeft) ? parent.left : undefined
    right: (alignment === QfBadge.Alignment.TopRight || alignment === QfBadge.Alignment.BottomRight) ? parent.right : undefined

    topMargin: badge.topMargin
    bottomMargin: badge.bottomMargin
    leftMargin: badge.leftMargin
    rightMargin: badge.rightMargin
  }

  Text {
    id: inText
    anchors.fill: parent
    font.pointSize: Math.max(8, height > 0 ? (text.length === 1 ? height / 2 : height / 2.2) : 8)
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment: Qt.AlignVCenter
    rotation: enableGradient ? -parent.rotation : 0
    // font.bold: true // maybe!
  }

  gradient: enableGradient ? greenBlueGradient : undefined
  rotation: enableGradient ? 45 : 0

  Gradient {
    id: greenBlueGradient
    orientation: Gradient.Horizontal
    GradientStop {
      position: 0.0
      color: Theme.mainColor
    }
    GradientStop {
      position: 0.4999
      color: Theme.mainColor
    }
    GradientStop {
      position: 0.5
      color: Theme.cloudColor
    }
    GradientStop {
      position: 1.0
      color: Theme.cloudColor
    }
  }
}
```


