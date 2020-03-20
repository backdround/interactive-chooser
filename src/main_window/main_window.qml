import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13

Window {
  id: main_window
  flags: Qt.FramelessWindowHint | Qt.Tool
  color: "transparent"

  property var unit_k: 108

  width: 6 * unit_k
  height: 8 * unit_k

  Style_init {}

  Chooser {
    anchors.fill: parent
  }
}
