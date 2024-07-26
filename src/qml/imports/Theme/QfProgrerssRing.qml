import QtQuick
import QtQuick.Controls

ProgressBar {
  id: control

  property int size: 56
  property real strokeWidth: 6
  property color color: Theme.mainColor
  property color backgroundColor: Theme.lightGray

  background: Rectangle {
    implicitWidth: control.size
    implicitHeight: control.size
    radius: control.width / 2
    color: "transparent"
    border.color: control.backgroundColor
    border.width: control.strokeWidth
  }

  onVisualPositionChanged: {
    canvas.requestPaint();
  }

  contentItem: Item {
    Canvas {
      id: canvas
      anchors.fill: parent
      antialiasing: true
      renderTarget: Canvas.Image
      property real radius: control.width / 2 - control.strokeWidth / 2

      onPaint: {
        var ctx = canvas.getContext("2d");
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.save();
        ctx.lineWidth = control.strokeWidth;
        ctx.strokeStyle = control.color;
        ctx.lineCap = "round";
        ctx.beginPath();
        ctx.arc(width / 2, height / 2, radius, -0.5 * Math.PI, -0.5 * Math.PI + control.visualPosition * 2 * Math.PI);
        ctx.stroke();
        ctx.closePath();
        ctx.restore();
      }
    }
  }
}
