import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13

Rectangle {
  id: root
  implicitHeight: relative_unit * 0.4
  implicitWidth: relative_unit * 2
  color: "transparent"
  clip: true

  property alias text: input_field.text

  Underline {
    activated: input_field.activeFocus
    active_color: Style.primary_alt
  }

  Input_field {
    id: input_field
    anchors.fill: parent
    horizontalAlignment: TextInput.AlignHCenter
    verticalAlignment: TextInput.AlignVCenter

    font.pointSize: relative_unit * 0.15

    sub_color: Style.primary_alt
  }

  onActiveFocusChanged: {
    if (activeFocus) {
      input_field.forceActiveFocus()
    }
  }
}
