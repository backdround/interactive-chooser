import QtQuick 2.13
import QtQuick.Layouts 1.13

Rectangle {
  color: Style.main_background
  Rectangle {
    anchors.fill: parent
    anchors.margins: relative_unit * 0.12
    color: "transparent"

    ColumnLayout {
      anchors.fill: parent
      spacing: relative_unit * 0.065

      Rectangle {
        Layout.fillWidth: true
        implicitHeight: relative_unit * 0.4
        color: Style.primary

        Input {
          anchors.centerIn: parent
          focus: true
          onTextChanged: {
            _model.user_input_changed(text)
            list.reset_current()
          }
        }
      }

      List {
        id: list
        font.pointSize: relative_unit * 0.125
        Layout.fillWidth: true
        Layout.fillHeight: true

        Shortcut {
          sequences: ["Ctrl+n", "Ctrl+j"]
          onActivated: list.incrementCurrentIndex()
        }

        Shortcut {
          sequences: ["Ctrl+p", "Ctrl+k"]
          onActivated: list.decrementCurrentIndex()
        }
        Shortcut {
          sequence: "Return"
          onActivated: list.action()
        }
      }
    }

  }
  border.width: 2
  border.color: Style.primary
}
