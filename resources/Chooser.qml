import QtQuick 2.13
import QtQuick.Layouts 1.13

Rectangle {
  color: Style.main_background
  Rectangle {
    anchors.fill: parent
    anchors.margins: unit_k * 0.12
    color: "transparent"

    ColumnLayout {
      anchors.fill: parent
      spacing: unit_k * 0.065

      Rectangle {
        Layout.fillWidth: true
        implicitHeight: unit_k * 0.4
        color: Style.primary

        Input {
          anchors.centerIn: parent
          focus: true
        }
      }

      List {
        id: list
        font.pointSize: unit_k * 0.125
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
          onActivated: print("index: " + list.currentIndex)
        }
      }
    }

  }
  border.width: 2
  border.color: Style.primary
}
