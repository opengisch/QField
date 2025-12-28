import QtQuick

/**
 * \ingroup qml
 */
Row {
  id: visibilityFadingRow
  property bool stateVisible: false
  visible: opacity > 0
  opacity: 0
  spacing: 4

  states: [
    State {
      when: visibilityFadingRow.stateVisible
      PropertyChanges {
        target: visibilityFadingRow
        opacity: 1.0
      }
    },
    State {
      when: !visibilityFadingRow.stateVisible
      PropertyChanges {
        target: visibilityFadingRow
        opacity: 0.0
      }
    }
  ]

  transitions: Transition {
    SequentialAnimation {
      NumberAnimation {
        target: visibilityFadingRow
        property: "opacity"
        duration: 250
      }
    }
  }
}
