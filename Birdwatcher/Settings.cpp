#include "StdAfx.h"
#include "Settings.h"
#include "Debug.h"


namespace Birdwatcher {

void Settings::InitializeCaptureDevices(void)
{
	// Get video devices list
	TCHAR name[256];
	int i = 0;
	while(m_pGraph->GetVideoDevice(i, name, 256) == NOERROR)
	{
		this->vidCapDeviceComboBox->Items->Add(gcnew String(name));
		i++;
	}
	if(i!=0)
		this->vidCapDeviceComboBox->SelectedIndex = m_pGraph->GetSelectedVideoDevice();

	// Get audio devices list
	i = 0;
	while(m_pGraph->GetAudioDevice(i, name, 256) == NOERROR)
	{
		this->audCapDeviceComboBox->Items->Add(gcnew String(name));
		i++;
	}
	if(i!=0)
		this->audCapDeviceComboBox->SelectedIndex = 0;
}

// FTP settings
void Settings::InitializeFTPSettings(void)
{
	BOOL bFTPEnabled = FALSE;
	m_pGraph->GetEnableFtpUpload(&bFTPEnabled);

	this->FTPEnableCheckBox->Checked::set(bFTPEnabled?true:false);
	EnableFTPSettings(bFTPEnabled?true:false);

	// FTP Server details
	char serverName[256];
	char userName[256];
	char passwd[256];
	char directory[256];
	m_pGraph->GetFtpServerInfos(serverName, userName, passwd, directory);
	//System::String s = serverName;
	String ^server= gcnew String(serverName);
	String ^user= gcnew String(userName);
	String ^passr= gcnew String(passwd);
	String ^dir= gcnew String(directory);
	this->serverNameBox->Text::set(server);
	this->userNameBox->Text::set(user);
	this->passwordBox->Text::set(passr);
	this->directoryBox->Text::set(dir);

	// Image file name
	char imageFileName[MAX_PATH];
	int fileExtension = 0;
	m_pGraph->GetImageFileName(imageFileName, &fileExtension);
	String ^filename= gcnew String(imageFileName);
	this->imageFileNameBox->Text::set(filename);

	this->imageFileNameComboBox->Items->Add(gcnew String(""));
	this->imageFileNameComboBox->Items->Add(gcnew String("timestamp (hhmmss)"));
	this->imageFileNameComboBox->SelectedIndex::set(fileExtension);

	// Images subdirectory
	char cSubDir[MAX_PATH];
	int subdirExtension = 0;
	m_pGraph->GetImageSubDir(cSubDir, &subdirExtension);
	String ^sSubDir = gcnew String(cSubDir);
	this->imageSubDirBox->Text::set(sSubDir);
	this->imageSubDirComboBox->Items->Add(gcnew String(""));
	this->imageSubDirComboBox->Items->Add(gcnew String("date (yyyymmdd)"));
	this->imageSubDirComboBox->SelectedIndex::set(subdirExtension);

	RefreshImageFileName();

	// Image size
	LONG w = 0, h = 0;
	m_pGraph->GetVideoSize(&w, &h);

	// imageSizeComboBox
	char csize[64];

	sprintf_s(csize, "%d x %d", w, h); // full size
	this->imageSizeComboBox->Items->Add(gcnew String(csize));
	sprintf_s(csize, "%d x %d", w/2, h/2); // half size
	this->imageSizeComboBox->Items->Add(gcnew String(csize));
	sprintf_s(csize, "%d x %d", w/4, h/4); // quarter size
	this->imageSizeComboBox->Items->Add(gcnew String(csize));

	int divider = 1;
	m_pGraph->GetImageSizeDivider(&divider);
	if(divider==1)
		this->imageSizeComboBox->SelectedIndex = 0;
	else if (divider == 2)
		this->imageSizeComboBox->SelectedIndex = 1;
	else if (divider == 4)
		this->imageSizeComboBox->SelectedIndex = 2;
	else
		DbgPrint("Image size divider invalid (%d)", divider);

	// Upload period
	unsigned long uploadPeriod = 0;
	m_pGraph->GetUploadPeriod(&uploadPeriod);

	int hours = (uploadPeriod/3600);
	uploadPeriod -= 3600*hours;
	int minutes = uploadPeriod/60;
	uploadPeriod -= 60*minutes;
	int seconds = uploadPeriod;
	this->uploadPeriodTimePicker->Value = System::DateTime(1978, 12, 15, hours, minutes, seconds, 0);

	// Jpeg quality
	this->jpegQualityScrollBar->Maximum += this->jpegQualityScrollBar->LargeChange - 1;
	this->jpegQualityScrollBar->Minimum = 0;
	int jpegQuality = 0;
	m_pGraph->GetJpegQuality(&jpegQuality);
	char cQuality[4];
	_itoa_s(jpegQuality, cQuality, 4, 10);
	String ^sQuality= gcnew String(cQuality);
	this->jpegQualityTextBox->Text = sQuality;
	this->jpegQualityScrollBar->Value = jpegQuality;

	BOOL bOverlayEnabled = FALSE;
	m_pGraph->GetOverlayEnable(&bOverlayEnabled);
	this->OverlayEnabledCheckBox->Checked::set(bOverlayEnabled?true:false);
}

void Settings::EnableFTPSettings(bool bEnable)
{
	this->FTPServerDetailsGroupBox->Enabled::set(bEnable);
	this->FTPMiscGroupBox->Enabled::set(bEnable);
}

void Settings::InitializeStillSettings(void)
{
	BOOL bStillCaptureEnabled = FALSE;
	m_pGraph->GetEnableStillCapture(&bStillCaptureEnabled);

	this->stillCaptureEnabledCheckBox->Checked::set(bStillCaptureEnabled?true:false);
	this->StillImgDetailsGroupBox->Enabled::set(bStillCaptureEnabled?true:false);

	// Still foder
	CHAR stillFolder[MAX_PATH];
	m_pGraph->GetStillDirectory(stillFolder);
	if(strcmp(stillFolder, "") == 0)
		this->stillFolderTextBox->Text = gcnew String(System::IO::Directory::GetCurrentDirectory());
	else
		this->stillFolderTextBox->Text = gcnew String(stillFolder);

	this->stillFolderBrowserDialog->SelectedPath = this->stillFolderTextBox->Text;

	CHAR stillFileName[MAX_PATH];
	int stillFileNameExtension;
	m_pGraph->GetStillImageFileName(stillFileName, &stillFileNameExtension);

	this->stillFileNameTextBox->Text::set(gcnew String(stillFileName));
	this->stillFileNameComboBox->Items->Add(gcnew String(""));
	this->stillFileNameComboBox->Items->Add(gcnew String("timestamp (hhmmss)"));
	this->stillFileNameComboBox->SelectedIndex::set(stillFileNameExtension);

	CHAR stillSubFolder[MAX_PATH];
	int stillSubFolderExtension;
	m_pGraph->GetStillImageSubDir(stillSubFolder, &stillSubFolderExtension);

	this->stillSubDirTextBox->Text::set(gcnew String(stillSubFolder));
	this->stillSubDirComboBox->Items->Add(gcnew String(""));
	this->stillSubDirComboBox->Items->Add(gcnew String("date (yyyymmdd)"));
	this->stillSubDirComboBox->SelectedIndex::set(stillSubFolderExtension);

	RefreshStillImageFileName();

	// Image size
	LONG w = 0, h = 0;
	m_pGraph->GetVideoSize(&w, &h);

	char csize[64];

	sprintf_s(csize, "%d x %d", w, h); // full size
	this->stillImageSizeComboBox->Items->Add(gcnew String(csize));
	sprintf_s(csize, "%d x %d", w/2, h/2); // half size
	this->stillImageSizeComboBox->Items->Add(gcnew String(csize));
	sprintf_s(csize, "%d x %d", w/4, h/4); // quarter size
	this->stillImageSizeComboBox->Items->Add(gcnew String(csize));

	int divider = 1;
	m_pGraph->GetStillImageSizeDivider(&divider);
	if(divider==1)
		this->stillImageSizeComboBox->SelectedIndex = 0;
	else if (divider == 2)
		this->stillImageSizeComboBox->SelectedIndex = 1;
	else if (divider == 4)
		this->stillImageSizeComboBox->SelectedIndex = 2;
	else
		DbgPrint("Image size divider invalid (%d)", divider);

	// Jpeg quality
	this->stillJpegQualityScrollBar->Maximum += this->stillJpegQualityScrollBar->LargeChange - 1;
	this->stillJpegQualityScrollBar->Minimum = 0;
	int stillJpegQuality = 0;
	m_pGraph->GetStillJpegQuality(&stillJpegQuality);
	char cQuality[4];
	_itoa_s(stillJpegQuality, cQuality, 4, 10);
	String ^sQuality= gcnew String(cQuality);
	this->stillJpegQualityTextBox->Text = sQuality;
	this->stillJpegQualityScrollBar->Value = stillJpegQuality;

	// Capture period
	unsigned long capturePeriod = 0;
	m_pGraph->GetStillCapturePeriod(&capturePeriod);

	int hours = (capturePeriod/3600);
	capturePeriod -= 3600*hours;
	int minutes = capturePeriod/60;
	capturePeriod -= 60*minutes;
	int seconds = capturePeriod;
	this->stillPeriodDateTimePicker->Value = System::DateTime(1978, 12, 15, hours, minutes, seconds, 0);

	// Overlay
	BOOL bOverlayEnabled = FALSE;
	m_pGraph->GetStillOverlayEnable(&bOverlayEnabled);
	this->StillOverlayEnabledCheckBox->Checked::set(bOverlayEnabled?true:false);
}

void Settings::InitializeRecordingSettings(void)
{
	bool bRecordingEnabled = m_pGraph->GetEnableRecording();

	this->recordingEnabledCheckBox->Checked::set(bRecordingEnabled?true:false);
	EnableRecordingSettings(bRecordingEnabled?true:false);

	// Video folder and filename
	WCHAR videoFolder[MAX_PATH];
	WCHAR videoSubdir[MAX_PATH];
	int subdirExtension;
	WCHAR fileName[MAX_PATH];
	int fileNameExtension;
	m_pGraph->GetVideoFileName(fileName, &fileNameExtension, videoFolder);
	m_pGraph->GetVideoSubdir(videoSubdir, &subdirExtension);

	if(wcscmp(videoFolder, L"") == 0)
		this->videoFolderTextBox->Text = gcnew String(System::IO::Directory::GetCurrentDirectory());
	else
		this->videoFolderTextBox->Text = gcnew String(videoFolder);
	
	this->videoFolderBrowserDialog->SelectedPath = this->videoFolderTextBox->Text;
	this->videoFilenameBox->Text::set(gcnew String(fileName));

	this->videoFileNameComboBox->Items->Add(gcnew String(""));
	this->videoFileNameComboBox->Items->Add(gcnew String("timestamp (hhmmss)"));
	this->videoFileNameComboBox->SelectedIndex::set(fileNameExtension);

	this->videoSubdirBox->Text::set(gcnew String(videoSubdir));
	this->videoSubdirComboBox->Items->Add(gcnew String(""));
	this->videoSubdirComboBox->Items->Add(gcnew String("date (yyyymmdd)"));
	this->videoSubdirComboBox->SelectedIndex::set(subdirExtension);

    RefreshVideoImageFileName();
	UpdateDriveInfo(this->videoFolderTextBox->Text);

	// Video file duration
	bool bLimitEnabled;
	unsigned long duration_sec;
	m_pGraph->GetVideoFileDuration(&bLimitEnabled, &duration_sec);

	int hours = (duration_sec/3600);
	duration_sec -= 3600*hours;
	int minutes = duration_sec/60;
	duration_sec -= 60*minutes;

	this->videoFileMaxDuration->Value = System::DateTime(1978, 12, 15, hours, minutes, 0, 0);

	this->limitVideoFileDurationcheckBox->Checked = bLimitEnabled;
	this->videoFileMaxDuration->Enabled = bLimitEnabled;

	// Get video codecs list
	TCHAR name[256];
	int i = 0;
	while(m_pGraph->GetVideoCompressor(i, name, 256) == NOERROR)
	{
		this->videoCodecsComboBox->Items->Add(gcnew String(name));
		i++;
	}
	if(i!=0)
		this->videoCodecsComboBox->SelectedIndex = m_pGraph->GetSelectedVideoCompressor();

	this->videoCodecPropPagesButton->Enabled = true;
	//this->CodecSettingsToolTip->IsBalloon = true;
}

void Settings::EnableRecordingSettings(bool bEnable)
{
	this->videoCompressionGroupBox->Enabled::set(bEnable);
	this->videoFileGroupBox->Enabled::set(bEnable);	
}

void Settings::InitializeWebServerSettings(void)
{
	bool enabled = m_pWebServer->IsEnabled();
	this->webServerEnableCheckBox->Checked = enabled;
	this->webServerGroupBox->Enabled::set(enabled);
}

void Settings::InitializeMotionDetectionSettings(void)
{
	BOOL bMotionDetectionEnabled = FALSE;
	m_pGraph->GetEnableMotionDetection(&bMotionDetectionEnabled);
	this->motionDetectionEnableCheckBox->Checked = bMotionDetectionEnabled?true:false;
	this->motionDetectionGroupBox->Enabled::set(bMotionDetectionEnabled);
}

void Settings::RefreshImageFileName()
{
	char cfileNameExtension[16] = "";
	if(this->imageFileNameComboBox->SelectedIndex == 1)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);		
		sprintf_s(cfileNameExtension, "%02d%02d%02d", time.wHour, time.wMinute, time.wSecond);
	}
	char cSubDirExtension[16] = "";
	if(this->imageSubDirComboBox->SelectedIndex == 1)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		sprintf_s(cSubDirExtension, "%04d%02d%02d\\", time.wYear, time.wMonth, time.wDay);
	}
	else if(this->imageSubDirBox->Text != "")
	{
		sprintf_s(cSubDirExtension, "%s", "\\");
	}

	String ^sfileNameExtension = gcnew String(cfileNameExtension);
	String ^sSubDirExtension = gcnew String(cSubDirExtension);
	this->exampleFileNameLabel->Text::set(this->imageSubDirBox->Text+sSubDirExtension+this->imageFileNameBox->Text+sfileNameExtension+L".jpg");
}
void Settings::RefreshStillImageFileName()
{
	char cfileNameExtension[16] = "";
	if(this->stillFileNameComboBox->SelectedIndex == 1)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);		
		sprintf_s(cfileNameExtension, "%02d%02d%02d", time.wHour, time.wMinute, time.wSecond);
	}
	char cSubDirExtension[16] = "";
	if(this->stillSubDirComboBox->SelectedIndex == 1)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		sprintf_s(cSubDirExtension, "%04d%02d%02d\\", time.wYear, time.wMonth, time.wDay);
	}
	else if(this->stillSubDirTextBox->Text != "")
	{
		sprintf_s(cSubDirExtension, "%s", "\\");
	}

	String ^sfileNameExtension = gcnew String(cfileNameExtension);
	String ^sSubDirExtension = gcnew String(cSubDirExtension);
	this->exampleStillFileNameLabel->Text::set(this->stillSubDirTextBox->Text+sSubDirExtension+this->stillFileNameTextBox->Text+sfileNameExtension+L".jpg");
}
void Settings::RefreshVideoImageFileName()
{
	char cfileNameExtension[16] = "";
	if(this->videoFileNameComboBox->SelectedIndex == 1)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);		
		sprintf_s(cfileNameExtension, "%02d%02d%02d", time.wHour, time.wMinute, time.wSecond);
	}
	char cSubdirExtension[16] = "";
	if(this->videoSubdirComboBox->SelectedIndex == 1)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		sprintf_s(cSubdirExtension, "%04d%02d%02d\\", time.wYear, time.wMonth, time.wDay);
	}
	else if(this->videoSubdirBox->Text != "")
	{
		sprintf_s(cSubdirExtension, "%s", "\\");
	}

	String ^sfileNameExtension = gcnew String(cfileNameExtension);
	String ^sSubdirExtension = gcnew String(cSubdirExtension);
	this->exampleVideoFileNameLabel->Text::set(this->videoSubdirBox->Text+sSubdirExtension+this->videoFilenameBox->Text+sfileNameExtension+L".avi");
}

void Settings::UpdateDriveInfo(String^ driveName)
{
	System::IO::DriveInfo^ dInfo = gcnew System::IO::DriveInfo(driveName);

	try
	{
		long long size = dInfo->TotalSize;
		size /= 1024*1024*1024;
		long long free = dInfo->AvailableFreeSpace ;
		free /= 1024*1024*1024;
		this->driveNameLabel->Text = "Drive " + dInfo->Name;
		this->driveCapacityLabel->Text =  "Capacity : " + size + " GB";
		this->driveCapacityLabel->ForeColor = System::Drawing::Color::Black;
		this->driveAvailableLabel->Text = "Available: " + free + " GB";

		this->videoDirectoryErrorProvider->SetError(this->videoFolderTextBox, "");
	}
	catch(Exception^ e)
	{
		this->driveNameLabel->Text = "Drive " + dInfo->Name;
		this->driveCapacityLabel->Text =  e->Message;
		this->driveCapacityLabel->ForeColor = System::Drawing::Color::Red;
		this->driveAvailableLabel->Text = "";

		this->videoDirectoryErrorProvider->SetIconAlignment(this->videoFolderTextBox, ErrorIconAlignment::MiddleLeft);
		this->videoDirectoryErrorProvider->SetError(this->videoFolderTextBox, e->Message);
	}
	

}

// Apply settings
void Settings::ApplySettings(void)
{
	bool bReconnect = ApplyCaptureDevices();
	bReconnect |= ApplyRecordingSettings();
	if(bReconnect)
	{
		m_pGraph->Stop();
		m_pGraph->Reconnect();
		m_dialogResult = System::Windows::Forms::DialogResult::OK;
	}

	ApplyFTPSettings();
	ApplyStillCaptureSettings();
	ApplyWebServerSettings();
	ApplyMotionDetectionSettings();
}

// return true if filter graph need to be rebuilt
bool Settings::ApplyCaptureDevices(void)
{
	bool bRebuildGraph = false;
	if(this->vidCapDeviceComboBox->SelectedIndex >= 0)
	{
		if(this->vidCapDeviceComboBox->SelectedIndex != m_pGraph->GetSelectedVideoDevice())
		{
			m_pGraph->SelectVideoDevice(this->vidCapDeviceComboBox->SelectedIndex);
			bRebuildGraph = true;
		}
	}
	return bRebuildGraph;
}
bool Settings::ApplyRecordingSettings()
{
	bool bRebuildGraph = false;
	bool bRecordingEnabled = this->recordingEnabledCheckBox->Checked;
	if(bRecordingEnabled != m_pGraph->GetEnableRecording())
	{
		m_pGraph->SetEnableRecording(bRecordingEnabled);
		bRebuildGraph = true;
	}
	
	WCHAR* videoFileName = (WCHAR*)(void*)Marshal::StringToHGlobalUni(this->videoFilenameBox->Text);
	WCHAR* videoDirectory = (WCHAR*)(void*)Marshal::StringToHGlobalUni(this->videoFolderTextBox->Text);
	m_pGraph->SetVideoFileName(videoFileName, this->videoFileNameComboBox->SelectedIndex, videoDirectory);
	Marshal::FreeHGlobal(IntPtr((void*)videoDirectory));
	Marshal::FreeHGlobal(IntPtr((void*)videoFileName));

	WCHAR* videoSubdir = (WCHAR*)(void*)Marshal::StringToHGlobalUni(this->videoSubdirBox->Text);
	m_pGraph->SetVideoSubdir(videoSubdir, this->videoSubdirComboBox->SelectedIndex);
	Marshal::FreeHGlobal(IntPtr((void*)videoSubdir));

	// File duration
	System::DateTime fileDuration = this->videoFileMaxDuration->Value;
	m_pGraph->SetVideoFileDuration(this->limitVideoFileDurationcheckBox->Checked, fileDuration.Minute*60+fileDuration.Hour*3600);

	if(this->videoCodecsComboBox->SelectedIndex != m_pGraph->GetSelectedVideoCompressor())
	{
		m_pGraph->SelectVideoCompressor(this->videoCodecsComboBox->SelectedIndex);
		bRebuildGraph = true;
	}

	return bRebuildGraph;
}

void Settings::ApplyFTPSettings(void)
{
	// Enable/Disable
	BOOL bFTPEnabled = this->FTPEnableCheckBox->Checked ? TRUE : FALSE;
	m_pGraph->SetEnableFtpUpload(bFTPEnabled);

	// FTP Server details
	char* serverName = (char*)(void*)Marshal::StringToHGlobalAnsi(this->serverNameBox->Text);
	char* userName = (char*)(void*)Marshal::StringToHGlobalAnsi(this->userNameBox->Text);
	char* passwd = (char*)(void*)Marshal::StringToHGlobalAnsi(this->passwordBox->Text);
	char* directory = (char*)(void*)Marshal::StringToHGlobalAnsi(this->directoryBox->Text);
	m_pGraph->SetFtpServerInfos(serverName, userName, passwd, directory);
	Marshal::FreeHGlobal(IntPtr((void*)serverName));
	Marshal::FreeHGlobal(IntPtr((void*)userName));
	Marshal::FreeHGlobal(IntPtr((void*)passwd));
	Marshal::FreeHGlobal(IntPtr((void*)directory));

	// Image filename
	char* filename = (char*)(void*)Marshal::StringToHGlobalAnsi(this->imageFileNameBox->Text);
	m_pGraph->SetImageFileName(filename, this->imageFileNameComboBox->SelectedIndex);
	Marshal::FreeHGlobal(IntPtr((void*)filename));

	// Images subdirectory
	char* subdir = (char*)(void*)Marshal::StringToHGlobalAnsi(this->imageSubDirBox->Text);
	m_pGraph->SetImageSubDir(subdir, this->imageSubDirComboBox->SelectedIndex);
	Marshal::FreeHGlobal(IntPtr((void*)subdir));

	// Image size
	int divider = 1;
	
	if(this->imageSizeComboBox->SelectedIndex == 0)
		m_pGraph->SetImageSizeDivider(1);
	else if (this->imageSizeComboBox->SelectedIndex == 1)
		m_pGraph->SetImageSizeDivider(2);
	else if (this->imageSizeComboBox->SelectedIndex == 2)
		m_pGraph->SetImageSizeDivider(4);
	else
		DbgPrint("Invalid imageSizeComboBox selection (%d)", this->imageSizeComboBox->SelectedIndex);


	// Jpeg quality
	int quality = Int32::Parse(this->jpegQualityTextBox->Text);
	if(quality>=0 && quality <=100)
		m_pGraph->SetJpegQuality(quality);

	// Upload period
	//unsigned long uploadPeriod = System::Convert::ToUInt32(this->uploadPeriodUpDown->Value);
	System::DateTime uploadTime = this->uploadPeriodTimePicker->Value;
	unsigned long uploadPeriod = uploadTime.Second+uploadTime.Minute*60+uploadTime.Hour*3600;
	m_pGraph->SetUploadPeriod(uploadPeriod);

	// Overlay
	BOOL bOverlayEnabled = this->OverlayEnabledCheckBox->Checked? TRUE : FALSE;
	m_pGraph->SetOverlayEnable(bOverlayEnabled);

}

void Settings::ApplyStillCaptureSettings(void)
{
	// Enable/Disable
	BOOL bStillCaptureEnabled = this->stillCaptureEnabledCheckBox->Checked ? TRUE : FALSE;
	m_pGraph->SetEnableStillCapture(bStillCaptureEnabled);

	// main directory
	char* stillFolder = (char*)(void*)Marshal::StringToHGlobalAnsi(this->stillFolderTextBox->Text);
	m_pGraph->SetStillDirectory(stillFolder);
	Marshal::FreeHGlobal(IntPtr((void*)stillFolder));

	// filename
	char* stillFilename = (char*)(void*)Marshal::StringToHGlobalAnsi(this->stillFileNameTextBox->Text);
	m_pGraph->SetStillImageFileName(stillFilename, this->stillFileNameComboBox->SelectedIndex);
	Marshal::FreeHGlobal(IntPtr((void*)stillFilename));

	// subdirectory
	char* stillSubdir = (char*)(void*)Marshal::StringToHGlobalAnsi(this->stillSubDirTextBox->Text);
	m_pGraph->SetStillImageSubDir(stillSubdir, this->stillSubDirComboBox->SelectedIndex);
	Marshal::FreeHGlobal(IntPtr((void*)stillSubdir));

	// image size
	int divider = 1;
	
	if(this->stillImageSizeComboBox->SelectedIndex == 0)
		m_pGraph->SetStillImageSizeDivider(1);
	else if (this->stillImageSizeComboBox->SelectedIndex == 1)
		m_pGraph->SetStillImageSizeDivider(2);
	else if (this->stillImageSizeComboBox->SelectedIndex == 2)
		m_pGraph->SetStillImageSizeDivider(4);
	else
		DbgPrint("Invalid stillImageSizeComboBox selection (%d)", this->stillImageSizeComboBox->SelectedIndex);

	// Jpeg quality
	int quality = Int32::Parse(this->stillJpegQualityTextBox->Text);
	if(quality>=0 && quality <=100)
		m_pGraph->SetStillJpegQuality(quality);

	// period
	System::DateTime captureTime = this->stillPeriodDateTimePicker->Value;
	unsigned long capturePeriod = captureTime.Second+captureTime.Minute*60+captureTime.Hour*3600;
	m_pGraph->SetStillCapturePeriod(capturePeriod);

	// Overlay
	BOOL bOverlayEnabled = this->StillOverlayEnabledCheckBox->Checked? TRUE : FALSE;
	m_pGraph->SetStillOverlayEnable(bOverlayEnabled);
}

void Settings::ApplyWebServerSettings(void)
{
	// Enable/Disable
	if(this->webServerEnableCheckBox->Checked)
		m_pWebServer->Enable();
		else
	m_pWebServer->Disable();
}

	
void Settings::ApplyMotionDetectionSettings(void)
{
	// Enable/Disable
	BOOL bMotionDetectionEnabled = this->motionDetectionEnableCheckBox->Checked ? TRUE : FALSE;
	m_pGraph->SetEnableMotionDetection(bMotionDetectionEnabled);
}


};