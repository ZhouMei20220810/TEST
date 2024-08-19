/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    // call default implementation to actually install YiShunYun.exe!
    component.createOperations();

    if (systemInfo.productType === "windows") 
	{
		var targetDir = "@TargetDir@";
        var exePath = targetDir + "/YiShunYun.exe";
		// 使用 Unicode 字符串来创建中文名称的快捷方式
        var startMenuLink = '@StartMenuDir@/\u6613\u821c\u4e91\u624b\u673a.lnk';
        var desktopLink = '@DesktopDir@/\u6613\u821c\u4e91\u624b\u673a.lnk';
		var iconPath = '@TargetDir@/logo.ico'; // 假设 logo.ico 文件与可执行文件位于同一目录下
        component.addOperation("CreateShortcut", exePath, startMenuLink,
            "workingDirectory=@TargetDir@", "iconPath="+iconPath, "description=run YiShunYun file");
		component.addOperation("CreateShortcut", exePath, desktopLink,
    "workingDirectory=" + targetDir, "iconPath="+iconPath, "description=Run YiShunYun file");
    }
}
