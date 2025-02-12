/***************************************************************************
 *   Copyright (c) 2015 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 51 Franklin Street,      *
 *   Fifth Floor, Boston, MA  02110-1301, USA                              *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#include <QFile>
#include <QMap>

#include "Branding.h"


using namespace App;

constexpr std::array filter = {
    "Application",
    "WindowTitle",
    "CopyrightInfo",
    "MaintainerUrl",
    "WindowIcon",
    "ProgramLogo",
    "ProgramIcons",
    "DesktopFileName",
    "StyleSheet",
    "BuildVersionMajor",
    "BuildVersionMinor",
    "BuildVersionPoint",
    "BuildRevision",
    "BuildRevisionDate",
    "BuildVersionSuffix",
    "BuildRepositoryURL",
    "AboutImage",
    "SplashScreen",
    "SplashAlignment",
    "SplashTextColor",
    "SplashInfoColor",
    "SplashInfoFont",
    "SplashInfoPosition",
    "SplashWarningColor",
    "StartWorkbench",
    "ExeName",
    "ExeVendor",
    "ExeVersion",
    "AppDataSkipVendor",
    "NavigationStyle",
    "UserParameterTemplate"
};

// Branding::Branding()
// {
// }

bool Branding::readFile(const QString& fn)
{
    QFile file(fn);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }
    if (!evaluateXML(&file, domDocument)) {
        return false;
    }
    file.close();
    return true;
}

Branding::XmlConfig Branding::getUserDefines() const
{
    XmlConfig cfg;
    QDomElement root = domDocument.documentElement();
    QDomElement child;
    if (!root.isNull()) {
        child = root.firstChildElement();
        while (!child.isNull()) {
            std::string name = child.localName().toLatin1().constData();
            std::string value = child.text().toUtf8().constData();
            if (std::find(filter.begin(), filter.end(), name) != filter.end()) {
                cfg[name] = value;
            }
            child = child.nextSiblingElement();
        }
    }
    return cfg;
}

bool Branding::evaluateXML(QIODevice* device, QDomDocument& xmlDocument)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!xmlDocument.setContent(device, true, &errorStr, &errorLine, &errorColumn)) {
        return false;
    }

    QDomElement root = xmlDocument.documentElement();
    if (root.tagName() != QLatin1String("Branding")) {
        return false;
    }
    else if (root.hasAttribute(QLatin1String("version"))) {
        QString attr = root.attribute(QLatin1String("version"));
        if (attr != QLatin1String("1.0")) {
            return false;
        }
    }

    return true;
}
