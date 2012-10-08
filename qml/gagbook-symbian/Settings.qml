/*
    Copyright (C) 2012  Dickson Leong
    This file is part of GagBook.

    GagBook is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see http://www.gnu.org/licenses/.
*/
import QtQuick 1.1

QtObject{
    id: settings

    property int selectedSection: settingsStorage.read("selectionSection", 0)
    onSelectedSectionChanged: settingsStorage.store("selectionSection", selectedSection)

    property int imageSize: settingsStorage.read("imageSize", 0)
    onImageSizeChanged: settingsStorage.store("imageSize", imageSize)

    property bool whiteTheme: settingsStorage.read("whiteTheme", false)
    onWhiteThemeChanged: settingsStorage.store("whiteTheme", whiteTheme)
}
