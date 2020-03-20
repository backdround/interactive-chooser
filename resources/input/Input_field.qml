import QtQuick 2.13
import QtQuick.Controls 2.13

TextInput {
  id: text_input

  color: "#2A2A2A"
  property var sub_color: "#444"

  cursorDelegate: Rectangle {
    width: unit_k * 0.0185
    color: sub_color

    Timer {
      id: blink_timer

      interval: 540
      running: text_input.activeFocus
      repeat: true

      onTriggered: parent.visible = !parent.visible
      onRunningChanged: parent.visible = running
    }
    onXChanged: blink_timer.restart()
  }

  selectionColor: sub_color
  Keys.onEscapePressed: deselect()
}
