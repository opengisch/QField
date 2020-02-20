import QtQuick 2.12

Row {
  id: visibilityFadingRow
  property bool stateVisible: undefined
  visible: opacity > 0

  states: [
      State { when: visibilityFadingRow.stateVisible;
              PropertyChanges { target: visibilityFadingRow; opacity: 1.0 }},
      State { when: !visibilityFadingRow.stateVisible;
              PropertyChanges { target: visibilityFadingRow; opacity: 0.0 }}
  ]

  transitions: Transition {
    SequentialAnimation {
      NumberAnimation {
        target: visibilityFadingRow
        property: "opacity"
        duration: 400
      }
    }
  }
}
