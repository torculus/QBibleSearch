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
        QRegularExpression nheb("NHEB");
        QRegularExpression kjv("KJV");
        if (nheb.match(translation).hasMatch()) {
            bible.setFileName(":/bibles/NHEB.txt");
            translation = "NHEB";
        } else if (kjv.match(translation).hasMatch()) {
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
            QRegularExpression re("(\\s|\-)" + search_term + "(\\s|\\n|,|\\;|\\:|\\.|\\!|\\?|\\-)", \
                                  QRegularExpression::CaseInsensitiveOption);

            QRegularExpressionMatch match = re.match(line);

            bool foundit = match.hasMatch();

            if (book == "Entire Bible") {

                if (foundit) {
                    verses.append(line);
                    verses.append("\n\n");

                    occurrences++;
                }
            } else if (book == "Old Testament") {
                // remove all New Testament verses
                QRegularExpression title(
                            "(Matthew|Mark|Luke|John|Acts|Romans|[1-2] Corinthians|Galatians|"
                            "Ephesians|Philippians|Colossians|[1-2] Thessalonians|[1-2] Timothy|"
                            "Titus|Philemon|Hebrews|James|[1-2] Peter|[1-3] James|Jude|Revelation)"
                            " [1-9]");
                QRegularExpressionMatch match = title.match(line);

                // if the New Testament is removed
                if (!match.hasMatch()) {
                    if (foundit) {
                        verses.append(line);
                        verses.append("\n\n");
                        occurrences++;
                    }
                }
            } else if (book == "New Testament") {
                // remove all Old Testament and apocrypha
                QRegularExpression title(
                            "(Genesis|Exodus|Leviticus|Numbers|Deuteronomy|Joshua|Judges|Ruth|"
                            "[1-2] Samuel|[1-2] Kings|[1-2] Chronicles|Ezra|Nehemiah|Esther|Job|Psalm|"
                            "Proverbs|Ecclesiastes|Song of Solomon|Isaiah|Jeremiah|Lamentations|Ezekiel|"
                            "Daniel|Hosea|Joel|Amos|Obadiah|Jonah|Micah|Nahum|Habakkuk|Zephaniah|Haggai|"
                            "Zechariah|Malachi) [1-9]");
                QRegularExpressionMatch match = title.match(line);

                // if the Old Testament is removed
                if (!match.hasMatch()) {
                    if (foundit) {
                        verses.append(line);
                        verses.append("\n\n");
                        occurrences++;
                    }
                }
            } else if (book == "Apocrypha/Deuterocanonical"){
                // remove all Old and New Testament verses
                QRegularExpression title(
                            "(Genesis|Exodus|Leviticus|Numbers|Deuteronomy|Joshua|Judges|Ruth|"
                            "[1-2] Samuel|[1-2] Kings|[1-2] Chronicles|Ezra|Nehemiah|Esther|Job|Psalm|"
                            "Proverbs|Ecclesiastes|Song of Solomon|Isaiah|Jeremiah|Lamentations|Ezekiel|"
                            "Daniel|Hosea|Joel|Amos|Obadiah|Jonah|Micah|Nahum|Habakkuk|Zephaniah|Haggai|"
                            "Zechariah|Malachi|Matthew|Mark|Luke|John|Acts|Romans|[1-2] Corinthians|Galatians|"
                            "Ephesians|Philippians|Colossians|[1-2] Thessalonians|[1-2] Timothy|Titus|Philemon|"
                            "Hebrews|James|[1-2] Peter|[1-3] James|Jude|Revelation) [1-9]");
                QRegularExpressionMatch match = title.match(line);

                // if the Old Testament is removed
                if (!match.hasMatch()) {
                    if (foundit) {
                        verses.append(line);
                        verses.append("\n\n");
                        occurrences++;
                    }
                }
            } else {
                QRegularExpression title(book + " [1-9]");
                QRegularExpressionMatch match = title.match(line);

                // this time, we want it if it DOES have a match
                if (match.hasMatch()) {
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
