/*
 * Copyright (c) 2018 Meltytech, LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef POSTJOBACTION_H
#define POSTJOBACTION_H

#include <QString>

class PostJobAction
{
public:
    virtual ~PostJobAction() {}
    virtual void doAction() = 0;
};

class FilePropertiesPostJobAction : public PostJobAction
{
public:
    FilePropertiesPostJobAction(const QString& srcFile, const QString& dstFile)
        : m_srcFile(srcFile)
        , m_dstFile(dstFile)
        {}
    virtual ~FilePropertiesPostJobAction() {}
    virtual void doAction();

private:
    QString m_srcFile;
    QString m_dstFile;
};

class ReverseFilePostJobAction : public FilePropertiesPostJobAction
{
public:
    ReverseFilePostJobAction(const QString& srcFile, const QString& dstFile, const QString& fileNameToRemove)
        : FilePropertiesPostJobAction(srcFile, dstFile)
        , m_fileNameToRemove(fileNameToRemove)
        {}
    void doAction();

private:
    QString m_fileNameToRemove;
};

#endif // POSTJOBACTION_H
