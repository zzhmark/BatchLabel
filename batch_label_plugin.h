/* batch_label_plugin.h
 * Label a batch of tif with swc in a folder, and generate a csv in a csv in the same folder.
 * 2020-3-8 : by zzh
 */
 
#ifndef __BATCH_LABEL_PLUGIN_H__
#define __BATCH_LABEL_PLUGIN_H__

#include <QtGui>
#include <v3d_interface.h>

class BatchLabel : public QObject, public V3DPluginInterface2_1
{
	Q_OBJECT
    Q_INTERFACES(V3DPluginInterface2_1)

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, V3DPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const V3DPluginArgList &input, V3DPluginArgList &output, V3DPluginCallback2 &callback, QWidget *parent);
};

#endif

