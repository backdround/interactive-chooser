import QtQml 2.13
import QtQuick 2.13
import QtQuick.Window 2.14

Item {
  Component.onCompleted: {
    Style.primary = "#5BB395"
    Style.primary_alt = "#202750"

    Style.main_background = "#77768B93"

    Style.unfocused_underline = "#886A6170"
    Style.list_element_background = "#FFF"

    Style.focused_text = "#000"
    Style.unfocused_text = "#3D3857"
  }
}
