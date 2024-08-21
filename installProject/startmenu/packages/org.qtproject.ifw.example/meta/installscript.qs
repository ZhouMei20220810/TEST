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
	if (!installer.addWizardPage(component, "Page", QInstaller.TargetDirectory))
        console.log("Could not add the dynamic page.");
	//加载自定义UI
    /*component.loaded.connect(this, addRegisterFileCheckBox);
    installer.installationFinished.connect(this, addOpenFileCheckBoxToFinishPage);
    installer.finishButtonClicked.connect(this, openRegisteredFileIfChecked);
    component.unusualFileType = generateUnusualFileType(5)*/

    installer.isSilent = true; // 设置静默模式
    // default constructor
    //var result = QMessageBox["question"]("test.quit", "Installer", "Do you want to quit the installer?<br>" +
    //        "This message box was created using JavaScript.", QMessageBox.Ok | QMessageBox.Cancel);
        //if (result == QMessageBox.Ok)
        {
            /*installer.setValue("FinishedText", "<font color='red' size=3>The installer was quit.</font>");*/

            //屏蔽页面显示
            installer.setDefaultPageVisible(QInstaller.TargetDirectory, true);
            installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
            installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
            installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
            installer.setDefaultPageVisible(QInstaller.PerformInstallation, true);
            installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
            /*gui.clickButton(buttons.NextButton);*/
            if (installer)
            {
                // 如果 installer.commandLineArguments 不存在，则初始化为空数组
                if (!installer.commandLineArguments)
                {
                    installer.commandLineArguments = [];
                }

                var args = installer.commandLineArguments;
                var result = QMessageBox["question"]("test.quit", "args", installer.commandLineArguments, QMessageBox.Ok | QMessageBox.Cancel);
                // 检查是否设置了静默安装标志
                if (args.indexOf('/S') >= 0 || args.indexOf('/s') >= 0)
                {
                    installer.isSilent = true;
                    var result = QMessageBox["question"]("test.quit", "args.indexOf('/S')", installer.commandLineArguments, QMessageBox.Ok | QMessageBox.Cancel);

                }

                // 检查是否指定了安装路径
                var installPathArgIndex = args.indexOf('/D');
                var result = QMessageBox["question"]("test.quit", "installPathArgIndex", installPathArgIndex, QMessageBox.Ok | QMessageBox.Cancel);

                if (installPathArgIndex >= 0)
                {
                    var result = QMessageBox["question"]("test.quit", "installPathArgIndex", installPathArgIndex, QMessageBox.Ok | QMessageBox.Cancel);

                    var installPath = args[installPathArgIndex + 1];
                    if (installPath)
                    {
                        // 设置安装目录
                        //installer.installDirectory = installPath;
                        //修改安装目录
                        installer.setValue('TargetDir', installPath);
                        var result = QMessageBox["question"]("test.quit", "installPath", installPath, QMessageBox.Ok | QMessageBox.Cancel);
                    }
                }
                else
                {
                    //installer.installDirectory = "D:\\Users\\Administrator";
                    //修改安装目录
                    installer.setValue('TargetDir', "D:\\Users\\Administrator\\YiShunYun");
                    //var result = QMessageBox["question"]("test.quit", "installPath", installer.installDirectory, QMessageBox.Ok | QMessageBox.Cancel);
                }

                // 输出调试信息
                console.log("Command Line Arguments:", args);
                console.log("Is Silent:", installer.isSilent);
                var result = QMessageBox["question"]("test.quit", "installPath", installer.isSilent, QMessageBox.Ok | QMessageBox.Cancel);
                console.log("Install Directory:", installer.installDirectory);
            }
            else
            {
                console.log("Warning: installer object not defined.");
            }
            console.log("handleCommandLineArguments leave", args);
        }
    /*else {
            installer.setValue("FinishedText",
                "<font color='green' size=3>The installer was not quit by JavaScript.</font>");
        }*/


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

Component.prototype.loaded = function ()
{
    var page = gui.pageByObjectName("DynamicPage");
    if (page != null) {
        console.log("Connecting the dynamic page entered signal.");
        page.entered.connect(Component.prototype.dynamicPageEntered);
    }
}

Component.prototype.dynamicPageEntered = function ()
{
    var pageWidget = gui.pageWidgetByObjectName("DynamicPage");
    if (pageWidget != null) {
        console.log("Setting the widgets label text.")
        pageWidget.m_pageLabel.text = "This is a dynamically created page.";
    }
}
