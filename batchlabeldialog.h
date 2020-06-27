#ifndef BATCHLABELDIALOG_H
#define BATCHLABELDIALOG_H

#include <QDialog>
#include <v3d_interface.h>
#include "basic_4dimage.h"

namespace Ui {
class BatchLabelDialog;
}

class BatchLabelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatchLabelDialog(QWidget *parent,
                              const QDir& folderpath,
                              const QFileInfoList& imgList,
                              const QFileInfoList& swcList,
                              V3DPluginCallback2* callback
                              );
    ~BatchLabelDialog();

private slots:
    void on_button_v3d_clicked();

    void on_button_previous_clicked();

    void on_button_next_clicked();

    void on_button_remove_clicked();

    bool on_button_save_clicked();

    void on_selection_changed(const QItemSelection& selected, const QItemSelection& deselected);

    void tab1();

    void tab2();

    void tab3();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::BatchLabelDialog *ui;
    QHash<QString, QFileInfo> imgFiles, swcFiles;
    QStringListModel listModel;
    QHash<QString, int> labels;
    V3DPluginCallback2* callback;
    QGraphicsScene scene_xy, scene_yz, scene_xz;
    QGraphicsPixmapItem* pxy;
    QGraphicsPixmapItem* pyz;
    QGraphicsPixmapItem* pxz;
    QList<QGraphicsLineItem*> txy, tyz, txz;
    QDir folderpath;
    QShortcut keyXY, keyYZ, keyXZ;
    bool saveflag;
    bool readImage(Image4DProxy<Image4DSimple> picture);
    void drawImage();
    void readSWC(QFileInfo swcfile);
    void drawSWC();
    void read(const QString& selectedfile);
    void resizeEvent(QResizeEvent* event);
};

#endif // BATCHLABELDIALOG_H
