#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDialog>
#include <QTextBrowser>
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

    this->setMaximumHeight(height());
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

        //QString search_verses = ui->lineEdit_2->text();
        //QString first_verse = book

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

            // find where the search term is a separate word or at the end of a verse
            bool x = line.contains(" " + search_term + " ", Qt::CaseInsensitive);
            bool y = line.contains(" " + search_term +"\n", Qt::CaseInsensitive);

            if (book == "Entire Bible") {

                if (x || y) {
                    verses.append(line);
                    verses.append("\n\n");

                    occurrences++;
                }
            } else if (book == "Old Testament") {
                // remove all New Testament verses
                if (line.contains("Matthew ")
                        || line.contains("Mark ")
                        || line.contains("Luke ")
                        || line.contains("John ")
                        || line.contains("Acts ")
                        || line.contains("Romans ")
                        || line.contains("1 Corinthians ")
                        || line.contains("2 Corinthians")
                        || line.contains("Galatians ")
                        || line.contains("Ephesians ")
                        || line.contains("Philippians ")
                        || line.contains("Colossians ")
                        || line.contains("1 Thessalonians ")
                        || line.contains("2 Thessalonians ")
                        || line.contains("1 Timothy ")
                        || line.contains("2 Timothy ")
                        || line.contains("Titus ")
                        || line.contains("Philemon ")
                        || line.contains("Hebrews ")
                        || line.contains("James ")
                        || line.contains("1 Peter ")
                        || line.contains("2 Peter ")
                        || line.contains("1 John ")
                        || line.contains("2 John ")
                        || line.contains("3 John ")
                        || line.contains("Jude ")
                        || line.contains("Revelation ")) {
                    //pass
                } else {
                    if (x || y) {
                        verses.append(line);
                        verses.append("\n\n");
                        occurrences++;
                    }
                }
            } else if (book == "New Testament") {

                if (line.contains("Genesis ")
                        || line.contains("Exodus ")
                        || line.contains("Leviticus ")
                        || line.contains("Numbers ")
                        || line.contains("Deuteronomy ")
                        || line.contains("Joshua ")
                        || line.contains("Judges ")
                        || line.contains("Ruth ")
                        || line.contains("1 Samuel ")
                        || line.contains("2 Samuel ")
                        || line.contains("1 Kings ")
                        || line.contains("2 Kings ")
                        || line.contains("1 Chronicles ")
                        || line.contains("2 Chronicles ")
                        || line.contains("Ezra ")
                        || line.contains("Nehemiah ")
                        || line.contains("Esther ")
                        || line.contains("Job ")
                        || line.contains("Psalm ")
                        || line.contains("Proverbs ")
                        || line.contains("Ecclesiastes ")
                        || line.contains("Song of Solomon ")
                        || line.contains("Isaiah ")
                        || line.contains("Jeremiah ")
                        || line.contains("Lamentations ")
                        || line.contains("Ezekiel ")
                        || line.contains("Daniel ")
                        || line.contains("Hosea ")
                        || line.contains("Joel ")
                        || line.contains("Amos ")
                        || line.contains("Obadiah ")
                        || line.contains("Jonah ")
                        || line.contains("Micah ")
                        || line.contains("Nahum ")
                        || line.contains("Habakkuk ")
                        || line.contains("Zephaniah ")
                        || line.contains("Haggai ")
                        || line.contains("Zechariah ")
                        || line.contains("Malachi ")) {
                    //pass
                } else {
                    if (x || y) {
                        verses.append(line);
                        verses.append("\n\n");
                        occurrences++;
                    }
                }
            } else if (book == "Apocrypha/Deuterocanonical"){

                if (line.contains("Genesis ")
                        || line.contains("Exodus ")
                        || line.contains("Leviticus ")
                        || line.contains("Numbers ")
                        || line.contains("Deuteronomy ")
                        || line.contains("Joshua ")
                        || line.contains("Judges ")
                        || line.contains("Ruth ")
                        || line.contains("1 Samuel ")
                        || line.contains("2 Samuel ")
                        || line.contains("1 Kings ")
                        || line.contains("2 Kings ")
                        || line.contains("1 Chronicles ")
                        || line.contains("2 Chronicles ")
                        || line.contains("Ezra ")
                        || line.contains("Nehemiah ")
                        || line.contains("Esther ")
                        || line.contains("Job ")
                        || line.contains("Psalm ")
                        || line.contains("Proverbs ")
                        || line.contains("Ecclesiastes ")
                        || line.contains("Song of Solomon ")
                        || line.contains("Isaiah ")
                        || line.contains("Jeremiah ")
                        || line.contains("Lamentations ")
                        || line.contains("Ezekiel ")
                        || line.contains("Daniel ")
                        || line.contains("Hosea ")
                        || line.contains("Joel ")
                        || line.contains("Amos ")
                        || line.contains("Obadiah ")
                        || line.contains("Jonah ")
                        || line.contains("Micah ")
                        || line.contains("Nahum ")
                        || line.contains("Habakkuk ")
                        || line.contains("Zephaniah ")
                        || line.contains("Haggai ")
                        || line.contains("Zechariah ")
                        || line.contains("Malachi ")
                        || line.contains("Matthew ")
                        || line.contains("Mark ")
                        || line.contains("Luke ")
                        || line.contains("John ")
                        || line.contains("Acts ")
                        || line.contains("Romans ")
                        || line.contains("1 Corinthians ")
                        || line.contains("2 Corinthians")
                        || line.contains("Galatians ")
                        || line.contains("Ephesians ")
                        || line.contains("Philippians ")
                        || line.contains("Colossians ")
                        || line.contains("1 Thessalonians ")
                        || line.contains("2 Thessalonians ")
                        || line.contains("1 Timothy ")
                        || line.contains("2 Timothy ")
                        || line.contains("Titus ")
                        || line.contains("Philemon ")
                        || line.contains("Hebrews ")
                        || line.contains("James ")
                        || line.contains("1 Peter ")
                        || line.contains("2 Peter ")
                        || line.contains("1 John ")
                        || line.contains("2 John ")
                        || line.contains("3 John ")
                        || line.contains("Jude ")
                        || line.contains("Revelation ")) {
                    //pass
                } else {
                    if (x || y) {
                        verses.append(line);
                        verses.append("\n\n");
                        occurrences++;
                    }
                }
            } else {
                if (line.contains(book + " ")) {
                    if (x || y) {
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

        QString window_text = QString(\
                    "%1 results found for \"" + search_term +"\" in \"" + translation +": "+ book +"\"").arg(occurrences);
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
