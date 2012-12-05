/****************************************************************************
 **
 ** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
 ** Contact: Qt Software Information (qt-info@nokia.com)
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License versions 2.0 or 3.0 as published by the Free
 ** Software Foundation and appearing in the file LICENSE.GPL included in
 ** the packaging of this file.  Please review the following information
 ** to ensure GNU General Public Licensing requirements will be met:
 ** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
 ** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
 ** exception, Nokia gives you certain additional rights. These rights
 ** are described in the Nokia Qt GPL Exception version 1.3, included in
 ** the file GPL_EXCEPTION.txt in this package.
 **
 ** Qt for Windows(R) Licensees
 ** As a special exception, Nokia, as the sole copyright holder for Qt
 ** Designer, grants users of the Qt/Eclipse Integration plug-in the
 ** right for the Qt/Eclipse Integration to link to functionality
 ** provided by Qt Designer and its related libraries.
 **
 ** If you are unsure which license is appropriate for your use, please
 ** contact the sales department at qt-sales@nokia.com.
 **
 ****************************************************************************/

#include <QtGui>

#include "elf-inspect-qt.h"
elf-inspect-qt::elf-inspect-qt()
{
    setupUi(this);
    connect(action_New, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(action_Open, SIGNAL(triggered()), this, SLOT(open()));
    connect(action_Save, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionSave_As, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(actionE_xit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionCu_t, SIGNAL(triggered()), textEdit, SLOT(cut()));
    connect(action_Copy, SIGNAL(triggered()), textEdit, SLOT(copy()));
    connect(action_Paste, SIGNAL(triggered()), textEdit, SLOT(paste()));
    connect(action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    action_New->setStatusTip(tr("Create a new file"));
    action_Open->setStatusTip(tr("Open an existing file"));
    action_Save->setStatusTip(tr("Save the document to disk"));
    actionSave_As->setStatusTip(tr("Save the document under a new name"));
    actionE_xit->setStatusTip(tr("Exit the application"));
    actionCu_t->setStatusTip(tr("Cut the current selection's contents to the "
                    "clipboard"));
    action_Copy->setStatusTip(tr("Copy the current selection's contents to the "
                    "clipboard"));
    action_Paste->setStatusTip(tr("Paste the clipboard's contents into the current "
                    "selection"));
    action_About->setStatusTip(tr("Show the application's About box"));
    actionAbout_Qt->setStatusTip(tr("Show the Qt library's About box"));

}

void elf-inspect-qt::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void elf-inspect-qt::newFile()
{
    if (maybeSave())
    {
        textEdit->clear();
        setCurrentFile("");
    }
}

void elf-inspect-qt::open()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool elf-inspect-qt::save()
{
    if (curFile.isEmpty())
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}

bool elf-inspect-qt::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
    return false;

    return saveFile(fileName);
}

void elf-inspect-qt::about()
{
    QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
                    "write modern GUI applications using Qt, with a menu bar, "
                    "toolbars, and a status bar."));
}

void elf-inspect-qt::documentWasModified()
{
    setWindowModified(textEdit->document()->isModified());
}

void elf-inspect-qt::readSettings()
{
    QSettings settings("Trolltech", "Application Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void elf-inspect-qt::writeSettings()
{
    QSettings settings("Trolltech", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool elf-inspect-qt::maybeSave()
{
    if (textEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        return save();
        else if (ret == QMessageBox::Cancel)
        return false;
    }
    return true;
}

void elf-inspect-qt::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(
                this,
                tr("Application"),
                tr("Cannot read file %1:\n%2.").arg(fileName).arg(
                        file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool elf-inspect-qt::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                tr("Cannot write file %1:\n%2.")
                .arg(fileName)
                .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void elf-inspect-qt::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}

QString elf-inspect-qt::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
