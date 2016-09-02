/*
* �������ڣ�2016-08-15
* ����޸ģ�2016-09-02
* ��    �ߣ�syf
* ��    ����
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTimer>


#include "image.h"
#include "camera.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void ShowImage(const Mat& image);

	void UpdataUI(void);
	

public slots:
	void OnTimer();

	void OpenImage();
	void OnImageSnap();

	void OnDrawMaskClicked();
	void OnUpClicked();
	void OnDownClicked();
	void OnLeftClicked();
	void OnRightClicked();
	void OnIncreaseClicked();
	void OnCutClicked();
	void OnIncreaseAngleClicked();
	void OnReduceAngleClicked();
	void OnCreateMask1Clicked();
	void OnCreateMask2Clicked();

	void OnThreshClicked();
	void OnToSrcClicked();
	void OnAdaptiveThreshClicked();
	void OnFilterClicked();
	void OnMaskClicked();
	void OnUserFilterClicked();
	void OnDilateClicked();
	void OnErodeClicked();


	void OnOpenCameraClicked();
	void OnCloseCameraClicked();
	void OnStartSnapClicked();
	void OnStopSnapClicked();
	void OnSoftwareTriggerClicked();
	void OnExposureModeChanged();
	void OnExporureValueChanged();
	void OnWhiteBalanceModeChanged();
	void OnWhiteBalanceRatioChanged();
	void OnWhiteBalanceChannelChanged();
protected:
	void mousePressEvent(QMouseEvent* event);

private:
    Ui::MainWindow *ui;
	QTimer* m_pTimer;

	Image* m_pImage;

	vector<Vec2i> m_vec;	// ��¼ͼ���еĽؾ�����

	Camera* m_pCamera;
};

#endif // MAINWINDOW_H
