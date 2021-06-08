import QtQml 2.3
import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import Qt.labs.settings 1.0

import QtCharts 2.3

import "widgets"

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: "Histogram"

    Connections {
        target: Histogram
        onGetProgress: progressBar.value = progress
        onGetValue: {
            setBar(value)
            setupRepeater(value)
        }
    }

    // Для сохранения позиции и размеров окна
    Settings {
        id: settings
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }

    ChartView {
        id: chart
        anchors {
            left: parent.left
            right: topWords.left
            bottom: parent.bottom
            top: mainRow.bottom
        }

        legend.alignment: Qt.AlignBottom
        antialiasing: true

        BarSeries {
            id: dictSeries

            axisX: BarCategoryAxis {
                id: categoryAxis
                labelsVisible: false
            }
            axisY: ValueAxis {
                id: yAxis
                min: 0
            }
        }
    }

    GroupBox {
        id: topWords
        title: "Top 15 words"
        width: 150

        Column {
            anchors.fill: parent
            Repeater {
                id: topWordsRepeater
                model: 15
                Custom_Label {
                    property string word: ""
                    property int count: 0
                    text: (index + 1) + '. ' + word + "  " + count
                }
            }
        }

        anchors {
            top: mainRow.bottom
            right: parent.right
            bottom: parent.bottom
            margins: 5
        }
    }

    RowLayout {
        id: mainRow
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 5
        }

        Custom_TextField {
            id: file_name
            Layout.fillWidth: true
        }

        Custom_ProgressBar {
            id: progressBar
            Layout.fillWidth: true
        }

        Custom_Button {
            text: "Выберите файл"

            onClicked: fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        nameFilters: [ "Text files (*.txt)" ]
        onAccepted: {
            file_name.text = fileDialog.fileUrl.toString()
            Histogram.setFile(fileDialog.fileUrl.toString())
        }
    }

    function setupRepeater(dict)
    {
        var j = 0
        for ( var i in dict )
        {
            topWordsRepeater.itemAt(j).word = i
            topWordsRepeater.itemAt(j).count = dict[i]
            j++
        }
    }

    function setBar(dict)
    {
        dictSeries.clear()
        var max = 0
        for ( var i in dict )
        {
            max = Math.max(dict[i], max)
            dictSeries.append(i, [dict[i]])
            categoryAxis.categories += i
        }

        yAxis.max = max
    }
}
