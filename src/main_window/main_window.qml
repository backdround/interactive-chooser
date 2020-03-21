import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13

Window {
  id: main_window
  flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
  modality: Qt.WindowModal
  color: "transparent"

  // Contains one tenth of screen height.
  property var relative_unit: 108

  Style_init {}

  Chooser {
    anchors.fill: parent
  }
}
