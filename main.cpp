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
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    a.setWindowIcon(QIcon(":/icons/QBibleSearch.svg"));

    /* The application icon was created by compositing the following public domain images:
     *
     * https://openclipart.org/detail/149527/bible
     *
     * https://openclipart.org/detail/140539/magnifying-glass
     *
     */

    return a.exec();
}
