/* batch_label_plugin.cpp
 * Label a batch of tif with swc in a folder, and generate a csv in a csv in the same folder.
 * 2020-3-8 : by zzh
 */

#include "v3d_message.h"
#include <vector>
#include "batch_label_plugin.h"
#include "batchlabeldialog.h"

using namespace std;
Q_EXPORT_PLUGIN2(batch_label, BatchLabel)
 
QStringList BatchLabel::menulist() const
{
	return QStringList() 
        <<tr("Open from directory..")
        <<tr("Readme..");
}

QStringList BatchLabel::funclist() const
{
	return QStringList()
		<<tr("help");
}

void BatchLabel::domenu(const QString &menu_name, V3DPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("Open from directory.."))
	{
            //input the folder path
        QString path = QFileDialog::getExistingDirectory(parent, tr("Open from directory.."));
        if(path.isEmpty()) return;
        QDir folderpath(path);
        if(!folderpath.exists()) return;
            //retrieve the img and swc files
        QFileInfoList imgList = folderpath.entryInfoList(QStringList() << "*.tif" << "*.tiff" << "*.nrrd" << "*.v3draw");
        QFileInfoList swcList = folderpath.entryInfoList(QStringList() << "*.swc" << "*.eswc");
        v3d_msg(QString("Detected img: %1\nDetected swc: %2").arg(imgList.size()).arg(swcList.size()));
        if (imgList.size() == 0)
        {
            v3d_msg(tr("No img to display."));
            return;
        }
        BatchLabelDialog* dialog = new BatchLabelDialog(parent, folderpath, imgList, swcList, &callback);
        dialog->show();
    }
    else
	{
        v3d_msg(tr("FUNCTION:\n"
                   "\tLabel a batch of image in a folder supported by Vaa3D interface with or without swc/eswc of consistent names,"
                   "and generate a csv..\n\n"
                   "SHORTCUTS:\n"
                   "\tUp/Down: switch between samples\n"
                   "\tF1, F2, F3: switch between planes\n"
                   "\t0-9: labels\n"
                   "\tBackspace: delete the label of current sample\n"
                   "\tCtrl+V: view in Vaa3D\n"
                   "\tCtrl+S: save\n\n"
                   "ATTENTION1: A current bug in Vaa3D 3D viewer may cause Vaa3D to crash, which locates in v3dr_control_signal.cpp.\n\n"
                   "You can choose to debug by canceling the signal/slot connections during the initControlValue of:\n"
                   "\txcmaxslider,\n\tycmaxslider,\n\tzcmaxslider\nwith glWidget temporarily, or wait for a fixed version of Vaa3D.\n\n"
                   "ATTENTION2: This plugin will alway ask you to save before quit without saving, unless ESC striken.\n\n"
                   "ATTENTION3: The current teem lib used by Vaa3D cannot open nrrd on Windows.\n\n"
                   "Developed by Zuohan Zhao, 2020-3-17, at Southeast University"
                   ));
	}
}

bool BatchLabel::dofunc(const QString & func_name, const V3DPluginArgList & input, V3DPluginArgList & output, V3DPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

    if (func_name == tr("help"))
	{
        v3d_msg(tr("FUNCTION:\n"
                   "\tLabel a batch of image in a folder supported by Vaa3D interface with or without swc/eswc of consistent names,"
                   "and generate a csv..\n\n"
                   "SHORTCUTS:\n"
                   "\tUp/Down: switch between samples\n"
                   "\tF1, F2, F3: switch between planes\n"
                   "\t0-9: labels\n"
                   "\tBackspace: delete the label of current sample\n"
                   "\tCtrl+V: view in Vaa3D\n"
                   "\tCtrl+S: save\n\n"
                   "ATTENTION1: A current bug in Vaa3D 3D viewer may cause Vaa3D to crash, which locates in v3dr_control_signal.cpp.\n\n"
                   "You can choose to debug by canceling the signal/slot connections during the initControlValue of:\n"
                   "\txcmaxslider,\n\tycmaxslider,\n\tzcmaxslider\nwith glWidget temporarily, or wait for a fixed version of Vaa3D.\n\n"
                   "ATTENTION2: This plugin will alway ask you to save before quit without saving, unless ESC striken.\n\n"
                   "ATTENTION3: The current teem lib used by Vaa3D cannot open nrrd on Windows.\n\n"
                   "Developed by Zuohan Zhao, 2020-3-17, at Southeast University"
                   ));
    } else if (func_name.toUpper() == tr("start").toUpper())
    {
        QString path = QFileDialog::getExistingDirectory(parent, tr("Open from directory.."));
        if(path.isEmpty()) return false;
        QDir folderpath(path);
        if(!folderpath.exists()) return false;
            //retrieve the img and swc files
        QFileInfoList imgList = folderpath.entryInfoList(QStringList() << "*.tif" << "*.tiff" << "*.nrrd" << "*.v3draw");
        QFileInfoList swcList = folderpath.entryInfoList(QStringList() << "*.swc" << "*.eswc");
        v3d_msg(QString("Detected img: %1\nDetected swc: %2").arg(imgList.size()).arg(swcList.size()));
        if (imgList.size() == 0)
        {
            v3d_msg(tr("No img to display."));
            return false;
        }
        BatchLabelDialog* dialog = new BatchLabelDialog(parent, folderpath, imgList, swcList, 0);
        dialog->exec();
    }
	else return false;

	return true;
}

