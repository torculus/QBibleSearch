/* Copyright 2018 Benjamin S. Osenbach
 *
 * This file is part of QBibleSearch.
 *
 * QBibleSearch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QBibleSearch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QBibleSearch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDialog>
#include <QFormLayout>
#include <QTextBrowser>
#include <QLabel>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_2->hide();

    // separate composite sections from the Old Testament
    ui->comboBox_2->insertSeparator(3);
    // separate the Old Testament from the New Testament
    ui->comboBox_2->insertSeparator(43); // separator is at position 43

    // prevent resizing the window
    this->setFixedSize(width(), height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::pair <QString, int> getVerses(QString include_books, QString line, bool foundit, int occurrences)
{
    QString results;

    QRegularExpression titles(include_books);
    QRegularExpressionMatch match = titles.match(line);

    if (match.hasMatch()) {
        if (foundit) {
            results.append(line);
            results.append("\n\n");

            // occurrences updates on every iteration of searchBible
            occurrences++;
        }
    }

    std::pair <QString, int> value;

    value.first = results;
    value.second = occurrences;

    return value;
}

void searchBible(Ui::MainWindow *ui) {
    QString search_term = ui->lineEdit->text();

    if (search_term != "") {

        QString translation = ui->comboBox->currentText();

        QString book = ui->comboBox_2->currentText();

        // possible feature in the next release
        //QString search_verses = ui->lineEdit_2->text();

        QFile bible;

        // allow for selecting different translations
        QRegularExpression nheb("NHEB");
        QRegularExpression kjv("KJV");

        if (nheb.match(translation).hasMatch()) {
            // disable the Include Apocrypha section
            ui->action_Include_Apocrypha->setVisible(false);

            bible.setFileName(":/bibles/NHEB.txt");
            translation = "NHEB";
        } else if (kjv.match(translation).hasMatch()) {
            // enable Apocrypha for KJV
            ui->action_Include_Apocrypha->setVisible(true);

            bible.setFileName(":/bibles/KJV.txt");
            translation = "KJV";
        }

        if (!bible.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        // read the entire Bible from the filestream
        QTextStream s1(&bible);

        QString verses;

        int occurrences = 0;

        while (!s1.atEnd()) {
            QString line = s1.readLine();

            // find where the search term is a separate word
            QRegularExpression re("(\\s|-)" + search_term + "(\\s|\\n|,|\\;|\\:|\\.|\\!|\\?|-)", \
                                  QRegularExpression::CaseInsensitiveOption);

            QRegularExpressionMatch match = re.match(line);

            bool foundit = match.hasMatch();

            if (book == "Entire Bible") {

                if (ui->action_Include_Apocrypha->isChecked()) {
                    // Catholic mode

                    if (foundit) {
                        verses.append(line);
                        verses.append("\n\n");

                        occurrences++;
                    }
                } else {
                    // Protestant mode

                    std::pair <QString, int> value = getVerses(
                                // exclude Apocryphal/Deuterocanonical books from the results
// !!!!!!!!!!!!!!!!!!!!!!!!
                                "[^(Tobit|Judith|Baruch)] [1-9]", line, foundit, occurrences);

                    verses.append(value.first);
                    occurrences = value.second;
                }

            } else if (book == "Old Testament") {

                if (ui->action_Include_Apocrypha->isChecked()) {
                    // Catholic mode

                    std::pair <QString, int> value = getVerses(
                                "(Genesis|Exodus|Leviticus|Numbers|Deuteronomy|Joshua|Judges|Ruth|"
// !!!!!!!!!!!!!!!!!!!!!!!!
                                "[1-2] Samuel|[1-2] Kings|[1-2] Chronicles|Ezra|Nehemiah|Esther|Job|Psalm|"
                                "Proverbs|Ecclesiastes|Song of Solomon|Isaiah|Jeremiah|Lamentations|Ezekiel|"
                                "Daniel|Hosea|Joel|Amos|Obadiah|Jonah|Micah|Nahum|Habakkuk|Zephaniah|Haggai|"
                                "Zechariah|Malachi) [1-9]", line, foundit, occurrences);

                    verses.append(value.first);
                    occurrences = value.second;

                } else {
                    // Protestant mode

                    std::pair <QString, int> value = getVerses(
                                "(Genesis|Exodus|Leviticus|Numbers|Deuteronomy|Joshua|Judges|Ruth|"
                                "[1-2] Samuel|[1-2] Kings|[1-2] Chronicles|Ezra|Nehemiah|Esther|Job|Psalm|"
                                "Proverbs|Ecclesiastes|Song of Solomon|Isaiah|Jeremiah|Lamentations|Ezekiel|"
                                "Daniel|Hosea|Joel|Amos|Obadiah|Jonah|Micah|Nahum|Habakkuk|Zephaniah|Haggai|"
                                "Zechariah|Malachi) [1-9]", line, foundit, occurrences);

                    verses.append(value.first);
                    occurrences = value.second;
                }

            } else if (book == "New Testament") {

                std::pair <QString, int> value = getVerses(
                            "(Matthew|Mark|Luke|John|Acts|Romans|[1-2] Corinthians|Galatians|"
                            "Ephesians|Philippians|Colossians|[1-2] Thessalonians|[1-2] Timothy|"
                            "Titus|Philemon|Hebrews|James|[1-2] Peter|[1-3] James|Jude|Revelation)"
                            " [1-9]", line, foundit, occurrences);

                verses.append(value.first);
                occurrences = value.second;

            } else if (book == "Apocrypha/Deuterocanonical"){

                std::pair <QString, int> value = getVerses(
                            "(Tobit|Judith|Baruch|Wisdom|Sirach/Ecclesiasticus|1 Maccabees|2 Maccabees)"
                            " [1-9]", line, foundit, occurrences);

                verses.append(value.first);
                occurrences = value.second;

            } else { // this condition is met if we're only searching through one particular book

                std::pair <QString, int> value = getVerses(book + " [1-9]", line, foundit, occurrences);

                verses.append(value.first);
                occurrences = value.second;
            }
        }

        bible.close();

        // create the search results window
        QDialog *sr = new QDialog();

        QString window_text;

        if (occurrences == 1) {
            // use proper English grammar
            window_text = QString("%1 result found for \"" + search_term \
                                          + "\" in \"" + translation +": "+ book +"\"").arg(occurrences);
        } else {
            window_text = QString("%1 results found for \"" + search_term \
                                          +"\" in \"" + translation +": "+ book +"\"").arg(occurrences);
        }

        sr->setWindowTitle(window_text);

        sr->setLayout(new QVBoxLayout());

        QTextBrowser *results = new QTextBrowser();

        if (occurrences == 0) {
            results->setText("No results found. Did you spell the term correctly?");
        } else {
            results->setText(verses);
        }

        sr->layout()->addWidget(results);

        sr->setMinimumWidth(500);

        sr->show();
    }
}

void MainWindow::on_lineEdit_returnPressed()
{
    searchBible(ui);
}

void MainWindow::on_lineEdit_2_returnPressed()
{
    searchBible(ui);
}

void MainWindow::on_actionAbout_QBibleSearch_triggered()
{
    QDialog *about_window = new QDialog();
    about_window->setWindowTitle("About QBibleSearch");

    QGridLayout *gridlayout = new QGridLayout();

    // include the application icon
    QPixmap icon(":/icons/QBibleSearch.png");
    QLabel *iconlabel = new QLabel();
    iconlabel->setPixmap(icon);
    gridlayout->addWidget(iconlabel, 0, 0, 2, 1);

    QLabel *heading = new QLabel();
    heading->setTextFormat(Qt::RichText);
    heading->setText("<h1>QBibleSearch 1.0</h1>");
    gridlayout->addWidget(heading, 0, 1, 1, 1);

    QLabel *ql = new QLabel();
    ql->setOpenExternalLinks(true);
    ql->setTextFormat(Qt::RichText);
    ql->setText("<p>Built with Qt 5.8.0<br /><br />"
                "Copyright (c) 2018 Benjamin S. Osenbach<br /><br />"
                "This program comes with absolutely no warranty. "
                "See the <a href=\"http://www.gnu.org/licenses/gpl-3.0.html\">"
                "GNU General Public License,version 3 or later</a> for details.<br /><br />"
                "<a href=\"https://github.com/torculus/QBibleSearch\">Project webpage</a></p>");
    ql->setWordWrap(true);
    gridlayout->addWidget(ql, 1, 1, 1, 1);

    about_window->setLayout(gridlayout);

    // prevent resizing the about dialog
    about_window->setFixedSize(500, 220);
    about_window->show();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_Include_Apocrypha_triggered()
{
    if (! ui->action_Include_Apocrypha->isChecked()) {
        // hide apocrypha
        ui->comboBox_2->removeItem(3);

        for (int i=0; i<17; i++) {
            // remove the apocrypha at the end
            ui->comboBox_2->removeItem(72);
        }

    } else {
        // show apocrypha
        ui->comboBox_2->insertItem(3, "Apocrypha/Deuterocanonical");

        // separate the New Testament from the Apocrypha
        ui->comboBox_2->insertSeparator(72);

        // add in the Apocrypha/Deuterocanonical books
        ui->comboBox_2->addItem("1 Esdras");
        ui->comboBox_2->addItem("2 Esdras");
        ui->comboBox_2->addItem("Tobit");
        ui->comboBox_2->addItem("Judith");
        ui->comboBox_2->addItem("Additions to Esther");
        ui->comboBox_2->addItem("Wisdom");
        ui->comboBox_2->addItem("Sirach/Ecclesiasticus");
        ui->comboBox_2->addItem("Baruch");
        ui->comboBox_2->addItem("Letter of Jeremiah");
        ui->comboBox_2->addItem("Prayer of Azariah");
        ui->comboBox_2->addItem("Susanna");
        ui->comboBox_2->addItem("Bel and the Dragon");
        ui->comboBox_2->addItem("Prayer of Manasseh");
        ui->comboBox_2->addItem("1 Macabees");
        ui->comboBox_2->addItem("2 Macabees");
    }
}

void MainWindow::closeEvent(QCloseEvent*)
{
    QApplication::quit();
}
