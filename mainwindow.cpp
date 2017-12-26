/* Copyright 2017 Benjamin S. Osenbach
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

    // separate large partitons of the Bible
    ui->comboBox_2->insertSeparator(4);
    //separate the Old Testament
    ui->comboBox_2->insertSeparator(44);
    //separate the New Testament
    ui->comboBox_2->insertSeparator(72);

    // prevent resizing the window
    this->setFixedSize(width(), height());
}

MainWindow::~MainWindow()
{
    delete ui;
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
        if (translation.indexOf("NHEB") != -1) {
            bible.setFileName(":/bibles/NHEB.txt");
            translation = "NHEB";
        } else if (translation.indexOf("KJV") != -1) {
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
            QRegularExpression re("\\s" + search_term + "\\s", QRegularExpression::CaseInsensitiveOption);

            QRegularExpressionMatch match = re.match(line);

            bool foundit = true;/*line.contains(" " + search_term + " ", Qt::CaseInsensitive)
                    || line.contains(" " + search_term +"\n", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + ":", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + ".", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + "-", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + ";", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + "!", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + "?", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + "/", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + "\"", Qt::CaseInsensitive)
                    || line.contains("\"" + search_term + " ", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + "\'", Qt::CaseInsensitive)
                    || line.contains(" " + search_term + ",", Qt::CaseInsensitive);*/

            if (book == "Entire Bible") {

                if (foundit) {
                    verses.append(line);
                    verses.append("\n\n");

                    occurrences++;
                }
            } else if (book == "Old Testament") {
                // remove all New Testament verses
                if (line.contains(QRegularExpression("Matthew [0-9]"))
                        || line.contains(QRegularExpression("Mark [0-9]"))
                        || line.contains(QRegularExpression("Luke [0-9]"))
                        || line.contains(QRegularExpression("John [0-9]"))
                        || line.contains(QRegularExpression("Acts [0-9]"))
                        || line.contains(QRegularExpression("Romans [0-9]"))
                        || line.contains(QRegularExpression("1 Corinthians [0-9]"))
                        || line.contains(QRegularExpression("2 Corinthians [0-9]"))
                        || line.contains(QRegularExpression("Galatians [0-9]"))
                        || line.contains(QRegularExpression("Ephesians [0-9]"))
                        || line.contains(QRegularExpression("Philippians [0-9]"))
                        || line.contains(QRegularExpression("Colossians [0-9]"))
                        || line.contains(QRegularExpression("1 Thessalonians [0-9]"))
                        || line.contains(QRegularExpression("2 Thessalonians [0-9]"))
                        || line.contains(QRegularExpression("1 Timothy [0-9]"))
                        || line.contains(QRegularExpression("2 Timothy [0-9]"))
                        || line.contains(QRegularExpression("Titus [0-9]"))
                        || line.contains(QRegularExpression("Philemon [0-9]"))
                        || line.contains(QRegularExpression("Hebrews [0-9]"))
                        || line.contains(QRegularExpression("James [0-9]"))
                        || line.contains(QRegularExpression("1 Peter [0-9]"))
                        || line.contains(QRegularExpression("2 Peter [0-9]"))
                        || line.contains(QRegularExpression("1 John [0-9]"))
                        || line.contains(QRegularExpression("2 John [0-9]"))
                        || line.contains(QRegularExpression("3 John [0-9]"))
                        || line.contains(QRegularExpression("Jude [0-9]"))
                        || line.contains(QRegularExpression("Revelation [0-9]")) ) {
                    //pass
                } else {
                    if (foundit) {
                        verses.append(line);
                        verses.append("\n\n");
                        occurrences++;
                    }
                }
            } else if (book == "New Testament") {

                if (line.contains(QRegularExpression("Genesis [0-9]"))
                        || line.contains(QRegularExpression("Exodus [0-9]"))
                        || line.contains(QRegularExpression("Leviticus [0-9]"))
                        || line.contains(QRegularExpression("Numbers [0-9]"))
                        || line.contains(QRegularExpression("Deuteronomy [0-9]"))
                        || line.contains(QRegularExpression("Joshua [0-9]"))
                        || line.contains(QRegularExpression("Judges [0-9]"))
                        || line.contains(QRegularExpression("Ruth [0-9]"))
                        || line.contains(QRegularExpression("1 Samuel [0-9]"))
                        || line.contains(QRegularExpression("2 Samuel [0-9]"))
                        || line.contains(QRegularExpression("1 Kings [0-9]"))
                        || line.contains(QRegularExpression("2 Kings [0-9]"))
                        || line.contains(QRegularExpression("1 Chronicles [0-9]"))
                        || line.contains(QRegularExpression("2 Chronicles [0-9]"))
                        || line.contains(QRegularExpression("Ezra [0-9]"))
                        || line.contains(QRegularExpression("Nehemiah [0-9]"))
                        || line.contains(QRegularExpression("Esther [0-9]"))
                        || line.contains(QRegularExpression("Job [0-9]"))
                        || line.contains(QRegularExpression("Psalm [0-9]"))
                        || line.contains(QRegularExpression("Proverbs [0-9]"))
                        || line.contains(QRegularExpression("Ecclesiastes [0-9]"))
                        || line.contains(QRegularExpression("Song of Solomon [0-9]"))
                        || line.contains(QRegularExpression("Isaiah [0-9]"))
                        || line.contains(QRegularExpression("Jeremiah [0-9]"))
                        || line.contains(QRegularExpression("Lamentations [0-9]"))
                        || line.contains(QRegularExpression("Ezekiel [0-9]"))
                        || line.contains(QRegularExpression("Daniel [0-9]"))
                        || line.contains(QRegularExpression("Hosea [0-9]"))
                        || line.contains(QRegularExpression("Joel [0-9]"))
                        || line.contains(QRegularExpression("Amos [0-9]"))
                        || line.contains(QRegularExpression("Obadiah [0-9]"))
                        || line.contains(QRegularExpression("Jonah [0-9]"))
                        || line.contains(QRegularExpression("Micah [0-9]"))
                        || line.contains(QRegularExpression("Nahum [0-9]"))
                        || line.contains(QRegularExpression("Habakkuk [0-9]"))
                        || line.contains(QRegularExpression("Zephaniah [0-9]"))
                        || line.contains(QRegularExpression("Haggai [0-9]"))
                        || line.contains(QRegularExpression("Zechariah [0-9]"))
                        || line.contains(QRegularExpression("Malachi [0-9]")) ) {
                    //pass
                } else {
                    if (foundit) {
                        verses.append(line);
                        verses.append("\n\n");
                        occurrences++;
                    }
                }
            } else if (book == "Apocrypha/Deuterocanonical"){

                if (line.contains(QRegularExpression("Genesis [0-9]"))
                        || line.contains(QRegularExpression("Exodus [0-9]"))
                        || line.contains(QRegularExpression("Leviticus [0-9]"))
                        || line.contains(QRegularExpression("Numbers [0-9]"))
                        || line.contains(QRegularExpression("Deuteronomy [0-9]"))
                        || line.contains(QRegularExpression("Joshua [0-9]"))
                        || line.contains(QRegularExpression("Judges [0-9]"))
                        || line.contains(QRegularExpression("Ruth [0-9]"))
                        || line.contains(QRegularExpression("1 Samuel [0-9]"))
                        || line.contains(QRegularExpression("2 Samuel [0-9]"))
                        || line.contains(QRegularExpression("1 Kings [0-9]"))
                        || line.contains(QRegularExpression("2 Kings [0-9]"))
                        || line.contains(QRegularExpression("1 Chronicles [0-9]"))
                        || line.contains(QRegularExpression("2 Chronicles [0-9]"))
                        || line.contains(QRegularExpression("Ezra [0-9]"))
                        || line.contains(QRegularExpression("Nehemiah [0-9]"))
                        || line.contains(QRegularExpression("Esther [0-9]"))
                        || line.contains(QRegularExpression("Job [0-9]"))
                        || line.contains(QRegularExpression("Psalm [0-9]"))
                        || line.contains(QRegularExpression("Proverbs [0-9]"))
                        || line.contains(QRegularExpression("Ecclesiastes [0-9]"))
                        || line.contains(QRegularExpression("Song of Solomon [0-9]"))
                        || line.contains(QRegularExpression("Isaiah [0-9]"))
                        || line.contains(QRegularExpression("Jeremiah [0-9]"))
                        || line.contains(QRegularExpression("Lamentations [0-9]"))
                        || line.contains(QRegularExpression("Ezekiel [0-9]"))
                        || line.contains(QRegularExpression("Daniel [0-9]"))
                        || line.contains(QRegularExpression("Hosea [0-9]"))
                        || line.contains(QRegularExpression("Joel [0-9]"))
                        || line.contains(QRegularExpression("Amos [0-9]"))
                        || line.contains(QRegularExpression("Obadiah [0-9]"))
                        || line.contains(QRegularExpression("Jonah [0-9]"))
                        || line.contains(QRegularExpression("Micah [0-9]"))
                        || line.contains(QRegularExpression("Nahum [0-9]"))
                        || line.contains(QRegularExpression("Habakkuk [0-9]"))
                        || line.contains(QRegularExpression("Zephaniah [0-9]"))
                        || line.contains(QRegularExpression("Haggai [0-9]"))
                        || line.contains(QRegularExpression("Zechariah [0-9]"))
                        || line.contains(QRegularExpression("Malachi [0-9]"))
                        || line.contains(QRegularExpression("Matthew [0-9]"))
                        || line.contains(QRegularExpression("Mark [0-9]"))
                        || line.contains(QRegularExpression("Luke [0-9]"))
                        || line.contains(QRegularExpression("John [0-9]"))
                        || line.contains(QRegularExpression("Acts [0-9]"))
                        || line.contains(QRegularExpression("Romans [0-9]"))
                        || line.contains(QRegularExpression("1 Corinthians [0-9]"))
                        || line.contains(QRegularExpression("2 Corinthians [0-9]"))
                        || line.contains(QRegularExpression("Galatians [0-9]"))
                        || line.contains(QRegularExpression("Ephesians [0-9]"))
                        || line.contains(QRegularExpression("Philippians [0-9]"))
                        || line.contains(QRegularExpression("Colossians [0-9]"))
                        || line.contains(QRegularExpression("1 Thessalonians [0-9]"))
                        || line.contains(QRegularExpression("2 Thessalonians [0-9]"))
                        || line.contains(QRegularExpression("1 Timothy [0-9]"))
                        || line.contains(QRegularExpression("2 Timothy [0-9]"))
                        || line.contains(QRegularExpression("Titus [0-9]"))
                        || line.contains(QRegularExpression("Philemon [0-9]"))
                        || line.contains(QRegularExpression("Hebrews [0-9]"))
                        || line.contains(QRegularExpression("James [0-9]"))
                        || line.contains(QRegularExpression("1 Peter [0-9]"))
                        || line.contains(QRegularExpression("2 Peter [0-9]"))
                        || line.contains(QRegularExpression("1 John [0-9]"))
                        || line.contains(QRegularExpression("2 John [0-9]"))
                        || line.contains(QRegularExpression("3 John [0-9]"))
                        || line.contains(QRegularExpression("Jude [0-9]"))
                        || line.contains(QRegularExpression("Revelation [0-9]")) ) {
                    //pass
                } else {
                    if (foundit) {
                        verses.append(line);
                        verses.append("\n\n");
                        occurrences++;
                    }
                }
            } else {
                if (line.contains( QRegularExpression(book + " [0-9]") ) ) {
                    if (foundit) {
                        verses.append(line);
                        verses.append("\n\n");

                        occurrences++;
                    }
                }
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
    about_window->setLayout(new QVBoxLayout());

    QLabel *ql = new QLabel("QBibleSearch\n\nVersion 1.0\n\n(c) 2017 Ben Osenbach <bsosenba@gmail.com>");

    about_window->layout()->addWidget(ql);

    about_window->setMaximumWidth(300);
    about_window->setMaximumHeight(300);

    about_window->show();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_Include_Apocrypha_triggered()
{
    //pass
}
