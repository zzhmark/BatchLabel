#include "batchlabeldialog.h"
#include "ui_batchlabeldialog.h"

void maxColorChoose(QRgb* pix, V3DLONG channel, v3d_uint8 value);

BatchLabelDialog::BatchLabelDialog(QWidget *parent,
                                   const QDir& folderpath,
                                   const QFileInfoList& imgList,
                                   const QFileInfoList& swcList,
                                   V3DPluginCallback2* callback
                                   ) :
    QDialog(parent),
    folderpath(folderpath),
    ui(new Ui::BatchLabelDialog),
    keyXY(tr("F1"), this), keyYZ(tr("F2"), this), keyXZ(tr("F3"), this),
    saveflag(false), pxy(0), pyz(0), pxz(0),
    callback(callback)
{
        // set up the hash tables for img and swc fileinfo
    foreach (QFileInfo img, imgList){
        imgFiles[img.completeBaseName()] = img;
        labels[img.completeBaseName()] = -1;
    }
    foreach (QFileInfo swc, swcList) swcFiles[swc.completeBaseName()] = swc;
    ui->setupUi(this);
        // model/view framework for the tri-view module
    ui->graphicsView_1->setScene(&scene_xy);
    ui->graphicsView_2->setScene(&scene_yz);
    ui->graphicsView_3->setScene(&scene_xz);
    ui->listView->setModel(&listModel);
        // model/view framework for the file name list module
    listModel.setStringList(imgFiles.keys());
    ui->listView->setCurrentIndex(listModel.index(0, 0));

        // set the buttons
    ui->buttonGroup->setId(ui->radioButton_0, 0);
    ui->buttonGroup->setId(ui->radioButton_1, 1);
    ui->buttonGroup->setId(ui->radioButton_2, 2);
    ui->buttonGroup->setId(ui->radioButton_3, 3);
    ui->buttonGroup->setId(ui->radioButton_4, 4);
    ui->buttonGroup->setId(ui->radioButton_5, 5);
    ui->buttonGroup->setId(ui->radioButton_6, 6);
    ui->buttonGroup->setId(ui->radioButton_7, 7);
    ui->buttonGroup->setId(ui->radioButton_8, 8);
    ui->buttonGroup->setId(ui->radioButton_9, 9);
    // select item in the list signals the slot
    connect(ui->listView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this,
            SLOT(on_selection_changed(QItemSelection, QItemSelection))
            );
        // shortcuts
    ui->radioButton_0->setShortcut(Qt::Key_0);
    ui->radioButton_1->setShortcut(Qt::Key_1);
    ui->radioButton_2->setShortcut(Qt::Key_2);
    ui->radioButton_3->setShortcut(Qt::Key_3);
    ui->radioButton_4->setShortcut(Qt::Key_4);
    ui->radioButton_5->setShortcut(Qt::Key_5);
    ui->radioButton_6->setShortcut(Qt::Key_6);
    ui->radioButton_7->setShortcut(Qt::Key_7);
    ui->radioButton_8->setShortcut(Qt::Key_8);
    ui->radioButton_9->setShortcut(Qt::Key_9);
    ui->button_next->setShortcut(Qt::Key_Down);
    ui->button_previous->setShortcut(Qt::Key_Up);
    ui->button_remove->setShortcut(tr("Backspace"));
    ui->button_save->setShortcut(tr("Ctrl+S"));
    ui->button_v3d->setShortcut(tr("Ctrl+V"));
    connect(&keyXY, SIGNAL(activated()), this, SLOT(tab1()));
    connect(&keyYZ, SIGNAL(activated()), this, SLOT(tab2()));
    connect(&keyXZ, SIGNAL(activated()), this, SLOT(tab3()));
    read(ui->listView->currentIndex().data().toString());       // select the first item in the list and display
    if (!callback) ui->button_v3d->setEnabled(false);
}

BatchLabelDialog::~BatchLabelDialog()
{
    delete ui;
}

void BatchLabelDialog::on_button_v3d_clicked()
{
        // display in vaa3d by generating an .ano file, delete later
    if (!callback)
    {
        v3d_msg(tr("This function is disabled when the plugin is called by dofunc."));
        return;
    }
    QFile temp(folderpath.absoluteFilePath(ui->listView->currentIndex().data().toString() + ".ano"));
    if (!temp.open(QIODevice::WriteOnly))
    {
        v3d_msg(tr("Fail to generate a temp .ano file to bring 3D Viewer to work."));
        return;
    }
    QTextStream out(&temp);
    if (!swcFiles[ui->listView->currentIndex().data().toString()].exists())
    {
        out << "SWCFILE=" << folderpath.absoluteFilePath("dummy.swc") << '\n';
        QFile dummy(folderpath.absoluteFilePath("dummy.swc"));
        if (dummy.open(QIODevice::WriteOnly)) dummy.close();
    } else out << "SWCFILE=" << swcFiles[ui->listView->currentIndex().data().toString()].absoluteFilePath() << '\n';
    out << "RAWIMG="  << imgFiles[ui->listView->currentIndex().data().toString()].absoluteFilePath();
    temp.close();
    callback->open3DViewerForLinkerFile(temp.fileName());
}

void BatchLabelDialog::on_button_previous_clicked()
{
    if (ui->listView->currentIndex().row() == 0) return;
    ui->listView->setCurrentIndex(listModel.index(ui->listView->currentIndex().row() - 1, 0));
}

void BatchLabelDialog::on_button_next_clicked()
{
    if (ui->listView->currentIndex().row() == listModel.rowCount() - 1) return;
    ui->listView->setCurrentIndex(listModel.index(ui->listView->currentIndex().row() + 1, 0));
}

void BatchLabelDialog::on_button_remove_clicked()
{
        // remove the current label
    if (ui->buttonGroup->checkedId() == -1) return;
    labels[ui->listView->currentIndex().data().toString()] = -1;
    ui->buttonGroup->setExclusive(false);
    ui->buttonGroup->checkedButton()->setChecked(false);
    ui->buttonGroup->setExclusive(true);
}

bool BatchLabelDialog::on_button_save_clicked()
{
        // save the labels
    ui->frame->setEnabled(false);
    ui->tabWidget->setEnabled(false);
    QString savepath = QFileDialog::getSaveFileName(this,
                                                    tr("Save the Labels.."),
                                                    folderpath.absolutePath(),
                                                    tr("*.csv")
                                                    );
    if (savepath.isEmpty())
    {
        ui->frame->setEnabled(true);
        ui->tabWidget->setEnabled(true);
        return false;
    }
    QFile savefile(savepath);
    if (!savefile.open(QIODevice::WriteOnly))
    {
        v3d_msg(tr("Fail to open the save path."));
        ui->frame->setEnabled(true);
        ui->tabWidget->setEnabled(true);
        return false;
    }
    QTextStream out(&savefile);
    QHashIterator<QString, int> i(labels);
    while (i.hasNext())
    {
        i.next();
        out << i.key() << '\t' << i.value() << '\n';
    }
    savefile.close();
    saveflag = true;
    ui->frame->setEnabled(true);
    ui->tabWidget->setEnabled(true);
    return true;
}

void BatchLabelDialog::on_selection_changed(const QItemSelection& selected, const QItemSelection& deselected)
{
        // reacts to any event that causes the selection in the listview, to change the tri-view and radiobutton status
    QString deselectedFile = ui->listView->model()->data(deselected.indexes()[0]).toString();
    QString selectedFile = ui->listView->model()->data(selected.indexes()[0]).toString();
    labels[deselectedFile] = ui->buttonGroup->checkedId();
    if (labels[selectedFile] != -1)
    {
        ui->buttonGroup->button(labels[selectedFile])->setChecked(true);
    } else if (ui->buttonGroup->checkedId() != -1)
    {
        ui->buttonGroup->setExclusive(false);
        ui->buttonGroup->checkedButton()->setChecked(false);
        ui->buttonGroup->setExclusive(true);
    }
    read(imgFiles[selectedFile].absoluteFilePath().toStdString().data());
}

void BatchLabelDialog::drawImage()
{
    if (!pxy && !pyz && !pxz) return;
    if (pxy)
    {
        pxy->setScale(std::min(scene_xy.width() / pxy->pixmap().width(), scene_xy.height() / pxy->pixmap().height()));
        pxy->setPos((scene_xy.width() - pxy->pixmap().width() * pxy->scale()) / 2,
                    (scene_xy.height() - pxy->pixmap().height() * pxy->scale()) / 2
                    );
    }
    if (pyz)
    {
        pyz->setScale(std::min(scene_yz.width() / pyz->pixmap().width(), scene_yz.height() / pyz->pixmap().height()));
        pyz->setPos((scene_yz.width() - pyz->pixmap().width() * pyz->scale()) / 2,
                    (scene_yz.height() - pyz->pixmap().height() * pyz->scale()) / 2
                    );
    }
    if (pxz)
    {
        pxz->setScale(std::min(scene_xz.width() / pxz->pixmap().width(), scene_xz.height() / pxz->pixmap().height()));
        pxz->setPos((scene_xz.width() - pxz->pixmap().width() * pxz->scale()) / 2,
                    (scene_xz.height() - pxz->pixmap().height() * pxz->scale()) / 2
                    );
    }
    return;
}

void maxColorChoose(QRgb* pix, V3DLONG channel, v3d_uint8 value)
{
    QColor color(*pix);
    switch (channel) {
    case 0:
        if (color.red() < value) color.setRed(value);
        break;
    case 1:
        if (color.green() < value) color.setGreen(value);
        break;
    case 2:
        if (color.blue() < value) color.setBlue(value);
        break;
    default:
        v3d_msg(QObject::tr("Channel error when allocating pixels."));
        break;
    }
    *pix = color.rgb();
}

void BatchLabelDialog::tab1()
{
    ui->tabWidget->setCurrentIndex(0);
}

void BatchLabelDialog::tab2()
{
    ui->tabWidget->setCurrentIndex(1);
}

void BatchLabelDialog::tab3()
{
    ui->tabWidget->setCurrentIndex(2);
}

void BatchLabelDialog::readSWC(QFileInfo swcfile)
{
        // read .swc file
    if(!swcfile.exists()) return;
    QFile file(swcfile.absoluteFilePath());
    if(!file.open(QIODevice::ReadOnly))
    {
        v3d_msg(tr("Fail to open the swc file."));
        return;
    }
    QTextStream reader(&file);
    QString readline(reader.readLine());

    while (readline.startsWith("#")) readline = reader.readLine();
    QHash<int, int> link;
    QHash<int, QVector3D> coord;
    while (!reader.atEnd())
    {
        QStringList split = readline.split(' ');
        int n = split[0].toInt();
        link[n] = split[6].toInt();
        coord[n] = QVector3D(split[2].toDouble(), split[3].toDouble(), split[4].toDouble());
        readline = reader.readLine();
    }
    file.close();
    QHashIterator<int, int> i(link);
    QPen pen(Qt::magenta, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    while (i.hasNext())
    {
        i.next();
        if(i.value() == -1) continue;
        QLineF line_xy(coord[i.value()].x(), coord[i.value()].y(), coord[i.key()].x(), coord[i.key()].y());
        QLineF line_yz(coord[i.value()].z(), coord[i.value()].y(), coord[i.key()].z(), coord[i.key()].y());
        QLineF line_xz(coord[i.value()].x(), coord[i.value()].z(), coord[i.key()].x(), coord[i.key()].z());
        txy.append(scene_xy.addLine(line_xy, pen));
        tyz.append(scene_yz.addLine(line_yz, pen));
        txz.append(scene_xz.addLine(line_xz, pen));
    }
}

void BatchLabelDialog::closeEvent(QCloseEvent* event)
{
    if (saveflag)
    {
        event->accept();
    } else
    {
        QMessageBox ask_save(QMessageBox::Question,
                             tr("Ask save.."),
                             tr("The labels are yet to be saved."),
                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                             this
                             );
        ask_save.setInformativeText("Do you want to save your changes?");
        ask_save.setDefaultButton(QMessageBox::Save);
        int ret = ask_save.exec();
        switch (ret)
        {
        case QMessageBox::Save:
            if (on_button_save_clicked()) event->accept(); else event->ignore();
            break;
        case QMessageBox::Discard:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        default:
            v3d_msg(tr("Something wrong with the ask save message box."));
            break;
        }
    }
}

void BatchLabelDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    scene_xy.setSceneRect(ui->graphicsView_1->rect().adjusted(1, 1, -1, -1));
    scene_yz.setSceneRect(ui->graphicsView_2->rect().adjusted(1, 1, -1, -1));
    scene_xz.setSceneRect(ui->graphicsView_3->rect().adjusted(1, 1, -1, -1));
    drawImage();
    drawSWC();
}

bool BatchLabelDialog::readImage(Image4DProxy<Image4DSimple> picture)
{
    // generate tir-view from the 3d img block
    QImage max_xy(picture.sx, picture.sy, QImage::Format_RGB32);
    QImage max_yz(picture.sz, picture.sy, QImage::Format_RGB32);
    QImage max_xz(picture.sx, picture.sz, QImage::Format_RGB32);
    if(max_xy.isNull() || max_yz.isNull() || max_xz.isNull()) {
        v3d_msg(tr("ERROR: cannot allocate memory to generate the tri-view"));
        return false;
    }
    max_xy.fill(0);
    max_yz.fill(0);
    max_xz.fill(0);
    switch (picture.sc) {
    case 1:
        Image4DProxy_foreach(picture, i, j, k, c)
        {
            v3d_uint8 value = *picture.at_uint8(i, j, k, c);
            for(int t = 0; t < 3; ++t)
            {
                maxColorChoose(reinterpret_cast<QRgb*>(max_xy.scanLine(j)) + i, t, value);
                maxColorChoose(reinterpret_cast<QRgb*>(max_yz.scanLine(j)) + k, t, value);
                maxColorChoose(reinterpret_cast<QRgb*>(max_xz.scanLine(k)) + i, t, value);
            }
        }
        break;
    case 3:
        Image4DProxy_foreach(picture, i, j, k, c)
        {
            v3d_uint8 value = *picture.at_uint8(i, j, k, c);
            maxColorChoose(reinterpret_cast<QRgb*>(max_xy.scanLine(j)) + i, c, value);
            maxColorChoose(reinterpret_cast<QRgb*>(max_yz.scanLine(j)) + k, c, value);
            maxColorChoose(reinterpret_cast<QRgb*>(max_xz.scanLine(k)) + i, c, value);
        }
        break;
    default:
        v3d_msg(tr("Image channels not supported."));
        return false;
    }
    pxy = scene_xy.addPixmap(QPixmap::fromImage(max_xy));
    pyz = scene_yz.addPixmap(QPixmap::fromImage(max_yz));
    pxz = scene_xz.addPixmap(QPixmap::fromImage(max_xz));
    return true;
}

void BatchLabelDialog::drawSWC()
{
        // draw swc onto the tri-view
        double scl1 = pxy->scale(), scl2 = pyz->scale(), scl3 = pxz->scale();
        QPointF pos1 = pxy->pos(), pos2 = pyz->pos(), pos3 = pxz->pos();
        QPen pen1(Qt::magenta, 3 / scl1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        QPen pen2(Qt::magenta, 3 / scl2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        QPen pen3(Qt::magenta, 3 / scl3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        foreach (QGraphicsLineItem* i, txy)
        {
            i->setScale(scl1);
            i->setPen(pen1);
            i->setPos(pos1);
        }
        foreach (QGraphicsLineItem* i, tyz)
        {
            i->setScale(scl2);
            i->setPen(pen2);
            i->setPos(pos2);
        }
        foreach (QGraphicsLineItem* i, txz)
        {
            i->setScale(scl3);
            i->setPen(pen3);
            i->setPos(pos3);
        }
}

void BatchLabelDialog::read(const QString &selectedfile)
{
    QFileInfo selectedImg = imgFiles[ui->listView->currentIndex().data().toString()];
    QFileInfo selectedSwc = swcFiles[ui->listView->currentIndex().data().toString()];
    Image4DSimple v3d_pic;
    v3d_pic.loadImage(selectedImg.absoluteFilePath().toStdString().data());
    if (!v3d_pic.isSuccess()) return;
    scene_xy.clear();
    scene_yz.clear();
    scene_xz.clear();
    pxy = pyz = pxz = 0;
    txy.clear();
    tyz.clear();
    txz.clear();
    if (readImage(&v3d_pic)) readSWC(selectedSwc);
    ui->tabWidget->setCurrentIndex(1);
    scene_yz.setSceneRect(ui->graphicsView_2->rect().adjusted(1, 1, -1, -1));
    ui->tabWidget->setCurrentIndex(2);
    scene_xz.setSceneRect(ui->graphicsView_3->rect().adjusted(1, 1, -1, -1));
    ui->tabWidget->setCurrentIndex(0);
    scene_xy.setSceneRect(ui->graphicsView_1->rect().adjusted(1, 1, -1, -1));
    drawImage();
    drawSWC();
}
