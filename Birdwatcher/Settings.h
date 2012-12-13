#pragma once

#include "FilterGraph.h"
#include "SchedulingForm.h"
#include "bwWebServer.h"
#include "Debug.h"

namespace Birdwatcher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;


	/// <summary>
	/// Summary for Settings
	/// </summary>
	public ref class Settings : public System::Windows::Forms::Form
	{
	public:
		Settings(CFilterGraph *pGraph, CWebServer *pWebServer)
		{
			m_pGraph = pGraph;
			m_pWebServer = pWebServer;
			InitializeComponent();
			//
			
			InitializeCaptureDevices();
			InitializeFTPSettings();
			InitializeStillSettings();
			InitializeRecordingSettings();
			InitializeMotionDetectionSettings();
			InitializeWebServerSettings();

			this->settingsApplyButton->Enabled = false;
			m_dialogResult = System::Windows::Forms::DialogResult::Cancel;
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Settings()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DialogResult m_dialogResult;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  CapDevicesTab;

	protected: 

	private: System::Windows::Forms::TabPage^  FTPtab;
	private: System::Windows::Forms::TabPage^  Recordingtab;


	private: System::Windows::Forms::GroupBox^  FTPServerDetailsGroupBox;




	private: System::Windows::Forms::TextBox^  serverNameBox;
	private: System::Windows::Forms::Label^  serverNameLabel;
	private: System::Windows::Forms::TextBox^  passwordBox;
	private: System::Windows::Forms::Label^  passwordLabel;
	private: System::Windows::Forms::TextBox^  userNameBox;
	private: System::Windows::Forms::Label^  userNameLabel;
	private: System::Windows::Forms::TextBox^  directoryBox;

	private: System::Windows::Forms::Label^  directoryLabel;
	private: System::Windows::Forms::GroupBox^  FTPMiscGroupBox;
	private: System::Windows::Forms::TextBox^  imageFileNameBox;
	private: System::Windows::Forms::Label^  imageFileNameLabel;


	private: System::Windows::Forms::Label^  uploadPeriodLabel;
	private: System::Windows::Forms::Label^  jpegQualityLabel;
	private: System::Windows::Forms::TextBox^  jpegQualityTextBox;
	private: System::Windows::Forms::HScrollBar^  jpegQualityScrollBar;
	private: System::Windows::Forms::Button^  settingsCancelButton;
	private: System::Windows::Forms::Button^  settingsOKButton;
	private: System::Windows::Forms::GroupBox^  audioDevicesGroupBox;
	private: System::Windows::Forms::GroupBox^  videoDevicesGroupBox;
	private: System::Windows::Forms::ComboBox^  vidCapDeviceComboBox;
	private: System::Windows::Forms::Label^  vidCapDeviceLabel;
	private: System::Windows::Forms::ComboBox^  audCapDeviceComboBox;
	private: System::Windows::Forms::Label^  audCapDeviceLabel;
	private: System::Windows::Forms::GroupBox^  videoCompressionGroupBox;
	private: System::Windows::Forms::ComboBox^  videoCodecsComboBox;
	private: System::Windows::Forms::Label^  videoCodecsLabel;
	private: System::Windows::Forms::Label^  exampleLabel;

	private: System::Windows::Forms::Label^  jpegExtensionLabel;
	private: System::Windows::Forms::ComboBox^  imageFileNameComboBox;
	private: System::Windows::Forms::Label^  plusLabel1;
	private: System::Windows::Forms::Label^  exampleFileNameLabel;
	private: System::Windows::Forms::ComboBox^  imageSizeComboBox;
	private: System::Windows::Forms::Label^  imageSizeLabel;
	private: System::Windows::Forms::ComboBox^  imageSubDirComboBox;

	private: System::Windows::Forms::Label^  plusLabel2;
	private: System::Windows::Forms::TextBox^  imageSubDirBox;
	private: System::Windows::Forms::Label^  imageSubDirLabel;
	private: System::Windows::Forms::CheckBox^  FTPEnableCheckBox;
	private: System::Windows::Forms::CheckBox^  recordingEnabledCheckBox;
	private: System::Windows::Forms::GroupBox^  videoFileGroupBox;
	private: System::Windows::Forms::TextBox^  videoFilenameBox;
	private: System::Windows::Forms::Label^  videoFileNameLabel;
	private: System::Windows::Forms::Label^  videoDirectoryLabel;

	private: System::Windows::Forms::FolderBrowserDialog^  videoFolderBrowserDialog;

	private: System::Windows::Forms::ErrorProvider^  videoDirectoryErrorProvider;
	private: System::Windows::Forms::Button^  browseButton;
	private: System::Windows::Forms::TextBox^  videoFolderTextBox;
	private: System::Windows::Forms::Label^  aviExtensionLabel;
	private: System::Windows::Forms::Label^  plusLabel3;
	private: System::Windows::Forms::ComboBox^  videoFileNameComboBox;
	private: System::Windows::Forms::DateTimePicker^  uploadPeriodTimePicker;
	private: System::Windows::Forms::Button^  videoCapturePropButton;
	private: System::Windows::Forms::Button^  videoCodecPropPagesButton;
	private: System::Windows::Forms::Button^  settingsApplyButton;
private: System::Windows::Forms::CheckBox^  limitVideoFileDurationcheckBox;
private: System::Windows::Forms::DateTimePicker^  videoFileMaxDuration;

private: System::Windows::Forms::Label^  driveNameLabel;
private: System::Windows::Forms::Label^  driveAvailableLabel;
private: System::Windows::Forms::Label^  driveCapacityLabel;
private: System::Windows::Forms::Button^  schedulingButton;
private: System::Windows::Forms::ToolTip^  CodecSettingsToolTip;
private: System::Windows::Forms::TabPage^  StillImagestab;
private: System::Windows::Forms::GroupBox^  StillImgDetailsGroupBox;
private: System::Windows::Forms::ComboBox^  stillSubDirComboBox;
private: System::Windows::Forms::Label^  plusLabel5;
private: System::Windows::Forms::TextBox^  stillSubDirTextBox;

private: System::Windows::Forms::Label^  stillSubDirCLabel;
private: System::Windows::Forms::Label^  jpegExtensionLabel2;
private: System::Windows::Forms::ComboBox^  stillFileNameComboBox;

private: System::Windows::Forms::Label^  plusLabel4;
private: System::Windows::Forms::TextBox^  stillFileNameTextBox;
private: System::Windows::Forms::Label^  stillFileNameLabel;
private: System::Windows::Forms::CheckBox^  stillCaptureEnabledCheckBox;
private: System::Windows::Forms::Button^  stillFolderButton;

private: System::Windows::Forms::TextBox^  stillFolderTextBox;
private: System::Windows::Forms::Label^  stillFolderLabel;


private: System::Windows::Forms::FolderBrowserDialog^  stillFolderBrowserDialog;
private: System::Windows::Forms::Label^  exampleStillFileNameLabel;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::DateTimePicker^  stillPeriodDateTimePicker;
private: System::Windows::Forms::Label^  stillPeriodLabel;
private: System::Windows::Forms::ComboBox^  stillImageSizeComboBox;
private: System::Windows::Forms::Label^  stillImageSizeLabel;
private: System::Windows::Forms::TextBox^  stillJpegQualityTextBox;
private: System::Windows::Forms::HScrollBar^  stillJpegQualityScrollBar;
private: System::Windows::Forms::Label^  stillJpegQualityLabel;
private: System::Windows::Forms::ComboBox^  videoSubdirComboBox;
private: System::Windows::Forms::Label^  plusLabel6;
private: System::Windows::Forms::TextBox^  videoSubdirBox;
private: System::Windows::Forms::Label^  videoSubdirLabel;
private: System::Windows::Forms::Label^  exampleVideoFileNameLabel;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::CheckBox^  StillOverlayEnabledCheckBox;
private: System::Windows::Forms::CheckBox^  OverlayEnabledCheckBox;
private: System::Windows::Forms::TextBox^  posYTextBox;
private: System::Windows::Forms::TextBox^  posXTextBox;
private: System::Windows::Forms::Label^  posYLabel;
private: System::Windows::Forms::Label^  posXLabel;



	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		CFilterGraph *m_pGraph;
private: System::Windows::Forms::CheckBox^  webServerEnableCheckBox;
private: System::Windows::Forms::TabPage^  WebServerTab;

private: System::Windows::Forms::TabPage^  MotionDetectionTab;
private: System::Windows::Forms::CheckBox^  motionDetectionEnableCheckBox;
private: System::Windows::Forms::GroupBox^  motionDetectionGroupBox;



		 CWebServer *m_pWebServer;
		/// </summary>

		void InitializeCaptureDevices(void);
		void InitializeFTPSettings(void);
		void EnableFTPSettings(bool bEnable);
		void InitializeStillSettings(void);
		void InitializeRecordingSettings(void);
		void EnableRecordingSettings(bool bEnable);
		void InitializeWebServerSettings(void);
		void InitializeMotionDetectionSettings(void);

		void RefreshImageFileName();
		void RefreshStillImageFileName();
		void RefreshVideoImageFileName();
		void UpdateDriveInfo(String^ driveName);

		void ApplySettings(void);
		bool ApplyCaptureDevices(void);
		bool ApplyRecordingSettings(void);
		void ApplyFTPSettings(void);
		void ApplyStillCaptureSettings(void);
		void ApplyMotionDetectionSettings(void);
private: System::ComponentModel::IContainer^  components;



#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->CapDevicesTab = (gcnew System::Windows::Forms::TabPage());
			this->audioDevicesGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->audCapDeviceComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->audCapDeviceLabel = (gcnew System::Windows::Forms::Label());
			this->videoDevicesGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->videoCapturePropButton = (gcnew System::Windows::Forms::Button());
			this->vidCapDeviceComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->vidCapDeviceLabel = (gcnew System::Windows::Forms::Label());
			this->FTPtab = (gcnew System::Windows::Forms::TabPage());
			this->FTPMiscGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->OverlayEnabledCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->uploadPeriodTimePicker = (gcnew System::Windows::Forms::DateTimePicker());
			this->imageSubDirComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->plusLabel2 = (gcnew System::Windows::Forms::Label());
			this->imageSubDirBox = (gcnew System::Windows::Forms::TextBox());
			this->imageSubDirLabel = (gcnew System::Windows::Forms::Label());
			this->imageSizeComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->imageSizeLabel = (gcnew System::Windows::Forms::Label());
			this->exampleFileNameLabel = (gcnew System::Windows::Forms::Label());
			this->exampleLabel = (gcnew System::Windows::Forms::Label());
			this->jpegExtensionLabel = (gcnew System::Windows::Forms::Label());
			this->imageFileNameComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->plusLabel1 = (gcnew System::Windows::Forms::Label());
			this->jpegQualityTextBox = (gcnew System::Windows::Forms::TextBox());
			this->jpegQualityScrollBar = (gcnew System::Windows::Forms::HScrollBar());
			this->jpegQualityLabel = (gcnew System::Windows::Forms::Label());
			this->uploadPeriodLabel = (gcnew System::Windows::Forms::Label());
			this->imageFileNameBox = (gcnew System::Windows::Forms::TextBox());
			this->imageFileNameLabel = (gcnew System::Windows::Forms::Label());
			this->FTPServerDetailsGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->directoryBox = (gcnew System::Windows::Forms::TextBox());
			this->directoryLabel = (gcnew System::Windows::Forms::Label());
			this->passwordBox = (gcnew System::Windows::Forms::TextBox());
			this->passwordLabel = (gcnew System::Windows::Forms::Label());
			this->userNameBox = (gcnew System::Windows::Forms::TextBox());
			this->userNameLabel = (gcnew System::Windows::Forms::Label());
			this->serverNameBox = (gcnew System::Windows::Forms::TextBox());
			this->serverNameLabel = (gcnew System::Windows::Forms::Label());
			this->StillImagestab = (gcnew System::Windows::Forms::TabPage());
			this->StillImgDetailsGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->posYTextBox = (gcnew System::Windows::Forms::TextBox());
			this->posXTextBox = (gcnew System::Windows::Forms::TextBox());
			this->posYLabel = (gcnew System::Windows::Forms::Label());
			this->posXLabel = (gcnew System::Windows::Forms::Label());
			this->StillOverlayEnabledCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->stillImageSizeComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->stillImageSizeLabel = (gcnew System::Windows::Forms::Label());
			this->stillJpegQualityTextBox = (gcnew System::Windows::Forms::TextBox());
			this->stillJpegQualityScrollBar = (gcnew System::Windows::Forms::HScrollBar());
			this->stillJpegQualityLabel = (gcnew System::Windows::Forms::Label());
			this->stillPeriodDateTimePicker = (gcnew System::Windows::Forms::DateTimePicker());
			this->stillPeriodLabel = (gcnew System::Windows::Forms::Label());
			this->exampleStillFileNameLabel = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->stillFolderButton = (gcnew System::Windows::Forms::Button());
			this->stillFolderTextBox = (gcnew System::Windows::Forms::TextBox());
			this->stillFolderLabel = (gcnew System::Windows::Forms::Label());
			this->stillSubDirComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->plusLabel5 = (gcnew System::Windows::Forms::Label());
			this->stillSubDirTextBox = (gcnew System::Windows::Forms::TextBox());
			this->stillSubDirCLabel = (gcnew System::Windows::Forms::Label());
			this->jpegExtensionLabel2 = (gcnew System::Windows::Forms::Label());
			this->stillFileNameComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->plusLabel4 = (gcnew System::Windows::Forms::Label());
			this->stillFileNameTextBox = (gcnew System::Windows::Forms::TextBox());
			this->stillFileNameLabel = (gcnew System::Windows::Forms::Label());
			this->Recordingtab = (gcnew System::Windows::Forms::TabPage());
			this->videoFileGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->exampleVideoFileNameLabel = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->videoSubdirComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->plusLabel6 = (gcnew System::Windows::Forms::Label());
			this->videoSubdirBox = (gcnew System::Windows::Forms::TextBox());
			this->videoSubdirLabel = (gcnew System::Windows::Forms::Label());
			this->schedulingButton = (gcnew System::Windows::Forms::Button());
			this->driveAvailableLabel = (gcnew System::Windows::Forms::Label());
			this->driveCapacityLabel = (gcnew System::Windows::Forms::Label());
			this->driveNameLabel = (gcnew System::Windows::Forms::Label());
			this->videoFileMaxDuration = (gcnew System::Windows::Forms::DateTimePicker());
			this->limitVideoFileDurationcheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->videoFileNameComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->plusLabel3 = (gcnew System::Windows::Forms::Label());
			this->aviExtensionLabel = (gcnew System::Windows::Forms::Label());
			this->browseButton = (gcnew System::Windows::Forms::Button());
			this->videoFolderTextBox = (gcnew System::Windows::Forms::TextBox());
			this->videoDirectoryLabel = (gcnew System::Windows::Forms::Label());
			this->videoFilenameBox = (gcnew System::Windows::Forms::TextBox());
			this->videoFileNameLabel = (gcnew System::Windows::Forms::Label());
			this->videoCompressionGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->videoCodecPropPagesButton = (gcnew System::Windows::Forms::Button());
			this->videoCodecsComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->videoCodecsLabel = (gcnew System::Windows::Forms::Label());
			this->WebServerTab = (gcnew System::Windows::Forms::TabPage());
			this->MotionDetectionTab = (gcnew System::Windows::Forms::TabPage());
			this->motionDetectionGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->FTPEnableCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->stillCaptureEnabledCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->recordingEnabledCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->settingsCancelButton = (gcnew System::Windows::Forms::Button());
			this->settingsOKButton = (gcnew System::Windows::Forms::Button());
			this->videoFolderBrowserDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->videoDirectoryErrorProvider = (gcnew System::Windows::Forms::ErrorProvider(this->components));
			this->settingsApplyButton = (gcnew System::Windows::Forms::Button());
			this->CodecSettingsToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->stillFolderBrowserDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->webServerEnableCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->motionDetectionEnableCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl1->SuspendLayout();
			this->CapDevicesTab->SuspendLayout();
			this->audioDevicesGroupBox->SuspendLayout();
			this->videoDevicesGroupBox->SuspendLayout();
			this->FTPtab->SuspendLayout();
			this->FTPMiscGroupBox->SuspendLayout();
			this->FTPServerDetailsGroupBox->SuspendLayout();
			this->StillImagestab->SuspendLayout();
			this->StillImgDetailsGroupBox->SuspendLayout();
			this->Recordingtab->SuspendLayout();
			this->videoFileGroupBox->SuspendLayout();
			this->videoCompressionGroupBox->SuspendLayout();
			this->MotionDetectionTab->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->videoDirectoryErrorProvider))->BeginInit();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl1->Controls->Add(this->CapDevicesTab);
			this->tabControl1->Controls->Add(this->FTPtab);
			this->tabControl1->Controls->Add(this->StillImagestab);
			this->tabControl1->Controls->Add(this->Recordingtab);
			this->tabControl1->Controls->Add(this->WebServerTab);
			this->tabControl1->Controls->Add(this->MotionDetectionTab);
			this->tabControl1->Location = System::Drawing::Point(12, 58);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(450, 348);
			this->tabControl1->TabIndex = 0;
			// 
			// CapDevicesTab
			// 
			this->CapDevicesTab->Controls->Add(this->audioDevicesGroupBox);
			this->CapDevicesTab->Controls->Add(this->videoDevicesGroupBox);
			this->CapDevicesTab->Location = System::Drawing::Point(4, 22);
			this->CapDevicesTab->Name = L"CapDevicesTab";
			this->CapDevicesTab->Padding = System::Windows::Forms::Padding(3);
			this->CapDevicesTab->Size = System::Drawing::Size(442, 322);
			this->CapDevicesTab->TabIndex = 0;
			this->CapDevicesTab->Text = L"Capture devices";
			this->CapDevicesTab->UseVisualStyleBackColor = true;
			// 
			// audioDevicesGroupBox
			// 
			this->audioDevicesGroupBox->Controls->Add(this->audCapDeviceComboBox);
			this->audioDevicesGroupBox->Controls->Add(this->audCapDeviceLabel);
			this->audioDevicesGroupBox->Location = System::Drawing::Point(6, 143);
			this->audioDevicesGroupBox->Name = L"audioDevicesGroupBox";
			this->audioDevicesGroupBox->Size = System::Drawing::Size(430, 129);
			this->audioDevicesGroupBox->TabIndex = 1;
			this->audioDevicesGroupBox->TabStop = false;
			this->audioDevicesGroupBox->Text = L"Audio Capture";
			// 
			// audCapDeviceComboBox
			// 
			this->audCapDeviceComboBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->audCapDeviceComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->audCapDeviceComboBox->FormattingEnabled = true;
			this->audCapDeviceComboBox->Location = System::Drawing::Point(93, 20);
			this->audCapDeviceComboBox->Name = L"audCapDeviceComboBox";
			this->audCapDeviceComboBox->Size = System::Drawing::Size(331, 21);
			this->audCapDeviceComboBox->TabIndex = 3;
			this->audCapDeviceComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::audCapDeviceComboBox_SelectedIndexChanged);
			// 
			// audCapDeviceLabel
			// 
			this->audCapDeviceLabel->AutoSize = true;
			this->audCapDeviceLabel->Location = System::Drawing::Point(7, 22);
			this->audCapDeviceLabel->Name = L"audCapDeviceLabel";
			this->audCapDeviceLabel->Size = System::Drawing::Size(79, 13);
			this->audCapDeviceLabel->TabIndex = 2;
			this->audCapDeviceLabel->Text = L"Capture device";
			// 
			// videoDevicesGroupBox
			// 
			this->videoDevicesGroupBox->Controls->Add(this->videoCapturePropButton);
			this->videoDevicesGroupBox->Controls->Add(this->vidCapDeviceComboBox);
			this->videoDevicesGroupBox->Controls->Add(this->vidCapDeviceLabel);
			this->videoDevicesGroupBox->Location = System::Drawing::Point(6, 6);
			this->videoDevicesGroupBox->Name = L"videoDevicesGroupBox";
			this->videoDevicesGroupBox->Size = System::Drawing::Size(430, 124);
			this->videoDevicesGroupBox->TabIndex = 0;
			this->videoDevicesGroupBox->TabStop = false;
			this->videoDevicesGroupBox->Text = L"Video Capture";
			// 
			// videoCapturePropButton
			// 
			this->videoCapturePropButton->Location = System::Drawing::Point(93, 48);
			this->videoCapturePropButton->Name = L"videoCapturePropButton";
			this->videoCapturePropButton->Size = System::Drawing::Size(75, 23);
			this->videoCapturePropButton->TabIndex = 2;
			this->videoCapturePropButton->Text = L"Source";
			this->videoCapturePropButton->UseVisualStyleBackColor = true;
			this->videoCapturePropButton->Click += gcnew System::EventHandler(this, &Settings::videoCapturePropButton_Click);
			// 
			// vidCapDeviceComboBox
			// 
			this->vidCapDeviceComboBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->vidCapDeviceComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->vidCapDeviceComboBox->FormattingEnabled = true;
			this->vidCapDeviceComboBox->Location = System::Drawing::Point(93, 20);
			this->vidCapDeviceComboBox->Name = L"vidCapDeviceComboBox";
			this->vidCapDeviceComboBox->Size = System::Drawing::Size(331, 21);
			this->vidCapDeviceComboBox->TabIndex = 1;
			this->vidCapDeviceComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::vidCapDeviceComboBox_SelectedIndexChanged);
			// 
			// vidCapDeviceLabel
			// 
			this->vidCapDeviceLabel->AutoSize = true;
			this->vidCapDeviceLabel->Location = System::Drawing::Point(7, 22);
			this->vidCapDeviceLabel->Name = L"vidCapDeviceLabel";
			this->vidCapDeviceLabel->Size = System::Drawing::Size(79, 13);
			this->vidCapDeviceLabel->TabIndex = 0;
			this->vidCapDeviceLabel->Text = L"Capture device";
			// 
			// FTPtab
			// 
			this->FTPtab->Controls->Add(this->FTPMiscGroupBox);
			this->FTPtab->Controls->Add(this->FTPServerDetailsGroupBox);
			this->FTPtab->Location = System::Drawing::Point(4, 22);
			this->FTPtab->Name = L"FTPtab";
			this->FTPtab->Padding = System::Windows::Forms::Padding(3);
			this->FTPtab->Size = System::Drawing::Size(442, 322);
			this->FTPtab->TabIndex = 1;
			this->FTPtab->Text = L"FTP";
			this->FTPtab->UseVisualStyleBackColor = true;
			// 
			// FTPMiscGroupBox
			// 
			this->FTPMiscGroupBox->Controls->Add(this->OverlayEnabledCheckBox);
			this->FTPMiscGroupBox->Controls->Add(this->uploadPeriodTimePicker);
			this->FTPMiscGroupBox->Controls->Add(this->imageSubDirComboBox);
			this->FTPMiscGroupBox->Controls->Add(this->plusLabel2);
			this->FTPMiscGroupBox->Controls->Add(this->imageSubDirBox);
			this->FTPMiscGroupBox->Controls->Add(this->imageSubDirLabel);
			this->FTPMiscGroupBox->Controls->Add(this->imageSizeComboBox);
			this->FTPMiscGroupBox->Controls->Add(this->imageSizeLabel);
			this->FTPMiscGroupBox->Controls->Add(this->exampleFileNameLabel);
			this->FTPMiscGroupBox->Controls->Add(this->exampleLabel);
			this->FTPMiscGroupBox->Controls->Add(this->jpegExtensionLabel);
			this->FTPMiscGroupBox->Controls->Add(this->imageFileNameComboBox);
			this->FTPMiscGroupBox->Controls->Add(this->plusLabel1);
			this->FTPMiscGroupBox->Controls->Add(this->jpegQualityTextBox);
			this->FTPMiscGroupBox->Controls->Add(this->jpegQualityScrollBar);
			this->FTPMiscGroupBox->Controls->Add(this->jpegQualityLabel);
			this->FTPMiscGroupBox->Controls->Add(this->uploadPeriodLabel);
			this->FTPMiscGroupBox->Controls->Add(this->imageFileNameBox);
			this->FTPMiscGroupBox->Controls->Add(this->imageFileNameLabel);
			this->FTPMiscGroupBox->Location = System::Drawing::Point(6, 133);
			this->FTPMiscGroupBox->Name = L"FTPMiscGroupBox";
			this->FTPMiscGroupBox->Size = System::Drawing::Size(430, 185);
			this->FTPMiscGroupBox->TabIndex = 1;
			this->FTPMiscGroupBox->TabStop = false;
			this->FTPMiscGroupBox->Text = L"Uploaded file details";
			// 
			// OverlayEnabledCheckBox
			// 
			this->OverlayEnabledCheckBox->AutoSize = true;
			this->OverlayEnabledCheckBox->Location = System::Drawing::Point(10, 162);
			this->OverlayEnabledCheckBox->Name = L"OverlayEnabledCheckBox";
			this->OverlayEnabledCheckBox->Size = System::Drawing::Size(146, 17);
			this->OverlayEnabledCheckBox->TabIndex = 19;
			this->OverlayEnabledCheckBox->Text = L"Enable timestamp overlay";
			this->OverlayEnabledCheckBox->UseVisualStyleBackColor = true;
			this->OverlayEnabledCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Settings::OverlayEnabledCheckBox_CheckedChanged);
			// 
			// uploadPeriodTimePicker
			// 
			this->uploadPeriodTimePicker->CustomFormat = L"HH\'h\' mm\'min\' ss\'sec\'";
			this->uploadPeriodTimePicker->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->uploadPeriodTimePicker->Location = System::Drawing::Point(81, 130);
			this->uploadPeriodTimePicker->Name = L"uploadPeriodTimePicker";
			this->uploadPeriodTimePicker->ShowUpDown = true;
			this->uploadPeriodTimePicker->Size = System::Drawing::Size(121, 20);
			this->uploadPeriodTimePicker->TabIndex = 13;
			this->uploadPeriodTimePicker->ValueChanged += gcnew System::EventHandler(this, &Settings::uploadPeriodTimePicker_ValueChanged);
			// 
			// imageSubDirComboBox
			// 
			this->imageSubDirComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->imageSubDirComboBox->FormattingEnabled = true;
			this->imageSubDirComboBox->Location = System::Drawing::Point(240, 46);
			this->imageSubDirComboBox->Name = L"imageSubDirComboBox";
			this->imageSubDirComboBox->Size = System::Drawing::Size(143, 21);
			this->imageSubDirComboBox->TabIndex = 10;
			this->imageSubDirComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::imageSubDirComboBox_SelectedIndexChanged);
			// 
			// plusLabel2
			// 
			this->plusLabel2->AutoSize = true;
			this->plusLabel2->Location = System::Drawing::Point(216, 50);
			this->plusLabel2->Name = L"plusLabel2";
			this->plusLabel2->Size = System::Drawing::Size(13, 13);
			this->plusLabel2->TabIndex = 17;
			this->plusLabel2->Text = L"+";
			// 
			// imageSubDirBox
			// 
			this->imageSubDirBox->Location = System::Drawing::Point(81, 46);
			this->imageSubDirBox->Name = L"imageSubDirBox";
			this->imageSubDirBox->Size = System::Drawing::Size(123, 20);
			this->imageSubDirBox->TabIndex = 9;
			this->imageSubDirBox->TextChanged += gcnew System::EventHandler(this, &Settings::imageSubDirBox_TextChanged);
			// 
			// imageSubDirLabel
			// 
			this->imageSubDirLabel->AutoSize = true;
			this->imageSubDirLabel->Location = System::Drawing::Point(7, 49);
			this->imageSubDirLabel->Name = L"imageSubDirLabel";
			this->imageSubDirLabel->Size = System::Drawing::Size(66, 13);
			this->imageSubDirLabel->TabIndex = 15;
			this->imageSubDirLabel->Text = L"Subdirectory";
			// 
			// imageSizeComboBox
			// 
			this->imageSizeComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->imageSizeComboBox->FormattingEnabled = true;
			this->imageSizeComboBox->Location = System::Drawing::Point(81, 96);
			this->imageSizeComboBox->Name = L"imageSizeComboBox";
			this->imageSizeComboBox->Size = System::Drawing::Size(111, 21);
			this->imageSizeComboBox->TabIndex = 11;
			this->imageSizeComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::imageSizeComboBox_SelectedIndexChanged);
			// 
			// imageSizeLabel
			// 
			this->imageSizeLabel->AutoSize = true;
			this->imageSizeLabel->Location = System::Drawing::Point(7, 99);
			this->imageSizeLabel->Name = L"imageSizeLabel";
			this->imageSizeLabel->Size = System::Drawing::Size(57, 13);
			this->imageSizeLabel->TabIndex = 13;
			this->imageSizeLabel->Text = L"Image size";
			// 
			// exampleFileNameLabel
			// 
			this->exampleFileNameLabel->AutoSize = true;
			this->exampleFileNameLabel->Location = System::Drawing::Point(152, 74);
			this->exampleFileNameLabel->Name = L"exampleFileNameLabel";
			this->exampleFileNameLabel->Size = System::Drawing::Size(0, 13);
			this->exampleFileNameLabel->TabIndex = 12;
			// 
			// exampleLabel
			// 
			this->exampleLabel->AutoSize = true;
			this->exampleLabel->Location = System::Drawing::Point(95, 74);
			this->exampleLabel->Name = L"exampleLabel";
			this->exampleLabel->Size = System::Drawing::Size(50, 13);
			this->exampleLabel->TabIndex = 11;
			this->exampleLabel->Text = L"Example:";
			// 
			// jpegExtensionLabel
			// 
			this->jpegExtensionLabel->AutoSize = true;
			this->jpegExtensionLabel->Location = System::Drawing::Point(389, 22);
			this->jpegExtensionLabel->Name = L"jpegExtensionLabel";
			this->jpegExtensionLabel->Size = System::Drawing::Size(33, 13);
			this->jpegExtensionLabel->TabIndex = 10;
			this->jpegExtensionLabel->Text = L"+ .jpg";
			// 
			// imageFileNameComboBox
			// 
			this->imageFileNameComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->imageFileNameComboBox->FormattingEnabled = true;
			this->imageFileNameComboBox->Location = System::Drawing::Point(240, 18);
			this->imageFileNameComboBox->Name = L"imageFileNameComboBox";
			this->imageFileNameComboBox->Size = System::Drawing::Size(143, 21);
			this->imageFileNameComboBox->TabIndex = 8;
			this->imageFileNameComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::imageFileNameComboBox_SelectedIndexChanged);
			// 
			// plusLabel1
			// 
			this->plusLabel1->AutoSize = true;
			this->plusLabel1->Location = System::Drawing::Point(216, 22);
			this->plusLabel1->Name = L"plusLabel1";
			this->plusLabel1->Size = System::Drawing::Size(13, 13);
			this->plusLabel1->TabIndex = 8;
			this->plusLabel1->Text = L"+";
			// 
			// jpegQualityTextBox
			// 
			this->jpegQualityTextBox->Location = System::Drawing::Point(386, 96);
			this->jpegQualityTextBox->Name = L"jpegQualityTextBox";
			this->jpegQualityTextBox->Size = System::Drawing::Size(40, 20);
			this->jpegQualityTextBox->TabIndex = 12;
			this->jpegQualityTextBox->TextChanged += gcnew System::EventHandler(this, &Settings::jpegQualityTextBox_TextChanged);
			this->jpegQualityTextBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Settings::jpegQualityTextBox_KeyPress);
			// 
			// jpegQualityScrollBar
			// 
			this->jpegQualityScrollBar->Location = System::Drawing::Point(267, 96);
			this->jpegQualityScrollBar->Name = L"jpegQualityScrollBar";
			this->jpegQualityScrollBar->Size = System::Drawing::Size(116, 20);
			this->jpegQualityScrollBar->TabIndex = 6;
			this->jpegQualityScrollBar->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Settings::jpegQualityScrollBar_Scroll);
			// 
			// jpegQualityLabel
			// 
			this->jpegQualityLabel->AutoSize = true;
			this->jpegQualityLabel->Location = System::Drawing::Point(201, 99);
			this->jpegQualityLabel->Name = L"jpegQualityLabel";
			this->jpegQualityLabel->Size = System::Drawing::Size(63, 13);
			this->jpegQualityLabel->TabIndex = 5;
			this->jpegQualityLabel->Text = L"Jpeg quality";
			// 
			// uploadPeriodLabel
			// 
			this->uploadPeriodLabel->AutoSize = true;
			this->uploadPeriodLabel->Location = System::Drawing::Point(7, 134);
			this->uploadPeriodLabel->Name = L"uploadPeriodLabel";
			this->uploadPeriodLabel->Size = System::Drawing::Size(70, 13);
			this->uploadPeriodLabel->TabIndex = 2;
			this->uploadPeriodLabel->Text = L"Upload every";
			// 
			// imageFileNameBox
			// 
			this->imageFileNameBox->Location = System::Drawing::Point(81, 18);
			this->imageFileNameBox->Name = L"imageFileNameBox";
			this->imageFileNameBox->Size = System::Drawing::Size(123, 20);
			this->imageFileNameBox->TabIndex = 7;
			this->imageFileNameBox->TextChanged += gcnew System::EventHandler(this, &Settings::imageFileNameBox_TextChanged);
			// 
			// imageFileNameLabel
			// 
			this->imageFileNameLabel->AutoSize = true;
			this->imageFileNameLabel->Location = System::Drawing::Point(7, 20);
			this->imageFileNameLabel->Name = L"imageFileNameLabel";
			this->imageFileNameLabel->Size = System::Drawing::Size(52, 13);
			this->imageFileNameLabel->TabIndex = 0;
			this->imageFileNameLabel->Text = L"File name";
			// 
			// FTPServerDetailsGroupBox
			// 
			this->FTPServerDetailsGroupBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->FTPServerDetailsGroupBox->Controls->Add(this->directoryBox);
			this->FTPServerDetailsGroupBox->Controls->Add(this->directoryLabel);
			this->FTPServerDetailsGroupBox->Controls->Add(this->passwordBox);
			this->FTPServerDetailsGroupBox->Controls->Add(this->passwordLabel);
			this->FTPServerDetailsGroupBox->Controls->Add(this->userNameBox);
			this->FTPServerDetailsGroupBox->Controls->Add(this->userNameLabel);
			this->FTPServerDetailsGroupBox->Controls->Add(this->serverNameBox);
			this->FTPServerDetailsGroupBox->Controls->Add(this->serverNameLabel);
			this->FTPServerDetailsGroupBox->Location = System::Drawing::Point(6, 6);
			this->FTPServerDetailsGroupBox->Name = L"FTPServerDetailsGroupBox";
			this->FTPServerDetailsGroupBox->Size = System::Drawing::Size(430, 122);
			this->FTPServerDetailsGroupBox->TabIndex = 0;
			this->FTPServerDetailsGroupBox->TabStop = false;
			this->FTPServerDetailsGroupBox->Text = L"FTP server details";
			// 
			// directoryBox
			// 
			this->directoryBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->directoryBox->Location = System::Drawing::Point(81, 87);
			this->directoryBox->Name = L"directoryBox";
			this->directoryBox->Size = System::Drawing::Size(343, 20);
			this->directoryBox->TabIndex = 7;
			this->directoryBox->TextChanged += gcnew System::EventHandler(this, &Settings::directoryBox_TextChanged);
			// 
			// directoryLabel
			// 
			this->directoryLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->directoryLabel->AutoSize = true;
			this->directoryLabel->Location = System::Drawing::Point(7, 88);
			this->directoryLabel->Name = L"directoryLabel";
			this->directoryLabel->Size = System::Drawing::Size(49, 13);
			this->directoryLabel->TabIndex = 7;
			this->directoryLabel->Text = L"Directory";
			// 
			// passwordBox
			// 
			this->passwordBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->passwordBox->Location = System::Drawing::Point(284, 55);
			this->passwordBox->Name = L"passwordBox";
			this->passwordBox->PasswordChar = '*';
			this->passwordBox->Size = System::Drawing::Size(140, 20);
			this->passwordBox->TabIndex = 6;
			this->passwordBox->TextChanged += gcnew System::EventHandler(this, &Settings::passwordBox_TextChanged);
			// 
			// passwordLabel
			// 
			this->passwordLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->passwordLabel->AutoSize = true;
			this->passwordLabel->Location = System::Drawing::Point(227, 57);
			this->passwordLabel->Name = L"passwordLabel";
			this->passwordLabel->Size = System::Drawing::Size(53, 13);
			this->passwordLabel->TabIndex = 5;
			this->passwordLabel->Text = L"Password";
			// 
			// userNameBox
			// 
			this->userNameBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->userNameBox->Location = System::Drawing::Point(81, 55);
			this->userNameBox->Name = L"userNameBox";
			this->userNameBox->Size = System::Drawing::Size(140, 20);
			this->userNameBox->TabIndex = 5;
			this->userNameBox->TextChanged += gcnew System::EventHandler(this, &Settings::userNameBox_TextChanged);
			// 
			// userNameLabel
			// 
			this->userNameLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->userNameLabel->AutoSize = true;
			this->userNameLabel->Location = System::Drawing::Point(7, 57);
			this->userNameLabel->Name = L"userNameLabel";
			this->userNameLabel->Size = System::Drawing::Size(60, 13);
			this->userNameLabel->TabIndex = 3;
			this->userNameLabel->Text = L"User Name";
			// 
			// serverNameBox
			// 
			this->serverNameBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->serverNameBox->Location = System::Drawing::Point(81, 23);
			this->serverNameBox->Name = L"serverNameBox";
			this->serverNameBox->Size = System::Drawing::Size(343, 20);
			this->serverNameBox->TabIndex = 4;
			this->serverNameBox->TextChanged += gcnew System::EventHandler(this, &Settings::serverNameBox_TextChanged);
			// 
			// serverNameLabel
			// 
			this->serverNameLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->serverNameLabel->AutoSize = true;
			this->serverNameLabel->Location = System::Drawing::Point(7, 23);
			this->serverNameLabel->Name = L"serverNameLabel";
			this->serverNameLabel->Size = System::Drawing::Size(67, 13);
			this->serverNameLabel->TabIndex = 1;
			this->serverNameLabel->Text = L"Server name";
			// 
			// StillImagestab
			// 
			this->StillImagestab->Controls->Add(this->StillImgDetailsGroupBox);
			this->StillImagestab->Location = System::Drawing::Point(4, 22);
			this->StillImagestab->Name = L"StillImagestab";
			this->StillImagestab->Padding = System::Windows::Forms::Padding(3);
			this->StillImagestab->Size = System::Drawing::Size(442, 322);
			this->StillImagestab->TabIndex = 3;
			this->StillImagestab->Text = L"Still images";
			this->StillImagestab->UseVisualStyleBackColor = true;
			// 
			// StillImgDetailsGroupBox
			// 
			this->StillImgDetailsGroupBox->Controls->Add(this->posYTextBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->posXTextBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->posYLabel);
			this->StillImgDetailsGroupBox->Controls->Add(this->posXLabel);
			this->StillImgDetailsGroupBox->Controls->Add(this->StillOverlayEnabledCheckBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillImageSizeComboBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillImageSizeLabel);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillJpegQualityTextBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillJpegQualityScrollBar);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillJpegQualityLabel);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillPeriodDateTimePicker);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillPeriodLabel);
			this->StillImgDetailsGroupBox->Controls->Add(this->exampleStillFileNameLabel);
			this->StillImgDetailsGroupBox->Controls->Add(this->label2);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillFolderButton);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillFolderTextBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillFolderLabel);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillSubDirComboBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->plusLabel5);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillSubDirTextBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillSubDirCLabel);
			this->StillImgDetailsGroupBox->Controls->Add(this->jpegExtensionLabel2);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillFileNameComboBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->plusLabel4);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillFileNameTextBox);
			this->StillImgDetailsGroupBox->Controls->Add(this->stillFileNameLabel);
			this->StillImgDetailsGroupBox->Location = System::Drawing::Point(6, 6);
			this->StillImgDetailsGroupBox->Name = L"StillImgDetailsGroupBox";
			this->StillImgDetailsGroupBox->Size = System::Drawing::Size(430, 312);
			this->StillImgDetailsGroupBox->TabIndex = 1;
			this->StillImgDetailsGroupBox->TabStop = false;
			this->StillImgDetailsGroupBox->Text = L"Still images details";
			// 
			// posYTextBox
			// 
			this->posYTextBox->Enabled = false;
			this->posYTextBox->Location = System::Drawing::Point(224, 257);
			this->posYTextBox->Name = L"posYTextBox";
			this->posYTextBox->Size = System::Drawing::Size(40, 20);
			this->posYTextBox->TabIndex = 43;
			this->posYTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->posYTextBox->TextChanged += gcnew System::EventHandler(this, &Settings::posYTextBox_TextChanged);
			this->posYTextBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Settings::posYTextBox_KeyPress);
			// 
			// posXTextBox
			// 
			this->posXTextBox->Enabled = false;
			this->posXTextBox->Location = System::Drawing::Point(224, 237);
			this->posXTextBox->Name = L"posXTextBox";
			this->posXTextBox->Size = System::Drawing::Size(40, 20);
			this->posXTextBox->TabIndex = 42;
			this->posXTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->posXTextBox->TextChanged += gcnew System::EventHandler(this, &Settings::posXTextBox_TextChanged);
			this->posXTextBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Settings::posXTextBox_KeyPress);
			// 
			// posYLabel
			// 
			this->posYLabel->AutoSize = true;
			this->posYLabel->Enabled = false;
			this->posYLabel->Location = System::Drawing::Point(166, 260);
			this->posYLabel->Name = L"posYLabel";
			this->posYLabel->Size = System::Drawing::Size(51, 13);
			this->posYLabel->TabIndex = 41;
			this->posYLabel->Text = L"y-position";
			// 
			// posXLabel
			// 
			this->posXLabel->AutoSize = true;
			this->posXLabel->Enabled = false;
			this->posXLabel->Location = System::Drawing::Point(166, 240);
			this->posXLabel->Name = L"posXLabel";
			this->posXLabel->Size = System::Drawing::Size(51, 13);
			this->posXLabel->TabIndex = 40;
			this->posXLabel->Text = L"x-position";
			// 
			// StillOverlayEnabledCheckBox
			// 
			this->StillOverlayEnabledCheckBox->AutoSize = true;
			this->StillOverlayEnabledCheckBox->Location = System::Drawing::Point(8, 240);
			this->StillOverlayEnabledCheckBox->Name = L"StillOverlayEnabledCheckBox";
			this->StillOverlayEnabledCheckBox->Size = System::Drawing::Size(146, 17);
			this->StillOverlayEnabledCheckBox->TabIndex = 39;
			this->StillOverlayEnabledCheckBox->Text = L"Enable timestamp overlay";
			this->StillOverlayEnabledCheckBox->UseVisualStyleBackColor = true;
			this->StillOverlayEnabledCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Settings::StillOverlayEnabledCheckBox_CheckedChanged);
			// 
			// stillImageSizeComboBox
			// 
			this->stillImageSizeComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->stillImageSizeComboBox->FormattingEnabled = true;
			this->stillImageSizeComboBox->Location = System::Drawing::Point(80, 158);
			this->stillImageSizeComboBox->Name = L"stillImageSizeComboBox";
			this->stillImageSizeComboBox->Size = System::Drawing::Size(111, 21);
			this->stillImageSizeComboBox->TabIndex = 36;
			this->stillImageSizeComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::stillImageSizeComboBox_SelectedIndexChanged);
			// 
			// stillImageSizeLabel
			// 
			this->stillImageSizeLabel->AutoSize = true;
			this->stillImageSizeLabel->Location = System::Drawing::Point(6, 161);
			this->stillImageSizeLabel->Name = L"stillImageSizeLabel";
			this->stillImageSizeLabel->Size = System::Drawing::Size(57, 13);
			this->stillImageSizeLabel->TabIndex = 38;
			this->stillImageSizeLabel->Text = L"Image size";
			// 
			// stillJpegQualityTextBox
			// 
			this->stillJpegQualityTextBox->Location = System::Drawing::Point(385, 158);
			this->stillJpegQualityTextBox->Name = L"stillJpegQualityTextBox";
			this->stillJpegQualityTextBox->Size = System::Drawing::Size(40, 20);
			this->stillJpegQualityTextBox->TabIndex = 37;
			this->stillJpegQualityTextBox->TextChanged += gcnew System::EventHandler(this, &Settings::stillJpegQualityTextBox_TextChanged);
			this->stillJpegQualityTextBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Settings::stillJpegQualityTextBox_KeyPress);
			// 
			// stillJpegQualityScrollBar
			// 
			this->stillJpegQualityScrollBar->Location = System::Drawing::Point(266, 158);
			this->stillJpegQualityScrollBar->Name = L"stillJpegQualityScrollBar";
			this->stillJpegQualityScrollBar->Size = System::Drawing::Size(116, 20);
			this->stillJpegQualityScrollBar->TabIndex = 35;
			this->stillJpegQualityScrollBar->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Settings::stillJpegQualityScrollBar_Scroll);
			// 
			// stillJpegQualityLabel
			// 
			this->stillJpegQualityLabel->AutoSize = true;
			this->stillJpegQualityLabel->Location = System::Drawing::Point(200, 161);
			this->stillJpegQualityLabel->Name = L"stillJpegQualityLabel";
			this->stillJpegQualityLabel->Size = System::Drawing::Size(63, 13);
			this->stillJpegQualityLabel->TabIndex = 34;
			this->stillJpegQualityLabel->Text = L"Jpeg quality";
			// 
			// stillPeriodDateTimePicker
			// 
			this->stillPeriodDateTimePicker->CustomFormat = L"HH\'h\' mm\'min\' ss\'sec\'";
			this->stillPeriodDateTimePicker->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->stillPeriodDateTimePicker->Location = System::Drawing::Point(77, 199);
			this->stillPeriodDateTimePicker->Name = L"stillPeriodDateTimePicker";
			this->stillPeriodDateTimePicker->ShowUpDown = true;
			this->stillPeriodDateTimePicker->Size = System::Drawing::Size(121, 20);
			this->stillPeriodDateTimePicker->TabIndex = 33;
			this->stillPeriodDateTimePicker->ValueChanged += gcnew System::EventHandler(this, &Settings::stillPeriodDateTimePicker_ValueChanged);
			// 
			// stillPeriodLabel
			// 
			this->stillPeriodLabel->AutoSize = true;
			this->stillPeriodLabel->Location = System::Drawing::Point(3, 203);
			this->stillPeriodLabel->Name = L"stillPeriodLabel";
			this->stillPeriodLabel->Size = System::Drawing::Size(73, 13);
			this->stillPeriodLabel->TabIndex = 32;
			this->stillPeriodLabel->Text = L"Capture every";
			// 
			// exampleStillFileNameLabel
			// 
			this->exampleStillFileNameLabel->AutoSize = true;
			this->exampleStillFileNameLabel->Location = System::Drawing::Point(133, 127);
			this->exampleStillFileNameLabel->Name = L"exampleStillFileNameLabel";
			this->exampleStillFileNameLabel->Size = System::Drawing::Size(0, 13);
			this->exampleStillFileNameLabel->TabIndex = 31;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(76, 127);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(50, 13);
			this->label2->TabIndex = 30;
			this->label2->Text = L"Example:";
			// 
			// stillFolderButton
			// 
			this->stillFolderButton->Location = System::Drawing::Point(397, 19);
			this->stillFolderButton->Name = L"stillFolderButton";
			this->stillFolderButton->Size = System::Drawing::Size(24, 22);
			this->stillFolderButton->TabIndex = 28;
			this->stillFolderButton->Text = L"...";
			this->stillFolderButton->UseVisualStyleBackColor = true;
			this->stillFolderButton->Click += gcnew System::EventHandler(this, &Settings::stillFolderButton_Click);
			// 
			// stillFolderTextBox
			// 
			this->stillFolderTextBox->Location = System::Drawing::Point(92, 20);
			this->stillFolderTextBox->Name = L"stillFolderTextBox";
			this->stillFolderTextBox->Size = System::Drawing::Size(302, 20);
			this->stillFolderTextBox->TabIndex = 27;
			// 
			// stillFolderLabel
			// 
			this->stillFolderLabel->AutoSize = true;
			this->stillFolderLabel->Location = System::Drawing::Point(3, 24);
			this->stillFolderLabel->Name = L"stillFolderLabel";
			this->stillFolderLabel->Size = System::Drawing::Size(84, 13);
			this->stillFolderLabel->TabIndex = 29;
			this->stillFolderLabel->Text = L"Images directory";
			// 
			// stillSubDirComboBox
			// 
			this->stillSubDirComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->stillSubDirComboBox->FormattingEnabled = true;
			this->stillSubDirComboBox->Location = System::Drawing::Point(238, 94);
			this->stillSubDirComboBox->Name = L"stillSubDirComboBox";
			this->stillSubDirComboBox->Size = System::Drawing::Size(143, 21);
			this->stillSubDirComboBox->TabIndex = 23;
			this->stillSubDirComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::stillSubDirComboBox_SelectedIndexChanged);
			// 
			// plusLabel5
			// 
			this->plusLabel5->AutoSize = true;
			this->plusLabel5->Location = System::Drawing::Point(214, 98);
			this->plusLabel5->Name = L"plusLabel5";
			this->plusLabel5->Size = System::Drawing::Size(13, 13);
			this->plusLabel5->TabIndex = 26;
			this->plusLabel5->Text = L"+";
			// 
			// stillSubDirTextBox
			// 
			this->stillSubDirTextBox->Location = System::Drawing::Point(79, 94);
			this->stillSubDirTextBox->Name = L"stillSubDirTextBox";
			this->stillSubDirTextBox->Size = System::Drawing::Size(123, 20);
			this->stillSubDirTextBox->TabIndex = 22;
			this->stillSubDirTextBox->TextChanged += gcnew System::EventHandler(this, &Settings::stillSubDirTextBox_TextChanged);
			// 
			// stillSubDirCLabel
			// 
			this->stillSubDirCLabel->AutoSize = true;
			this->stillSubDirCLabel->Location = System::Drawing::Point(5, 97);
			this->stillSubDirCLabel->Name = L"stillSubDirCLabel";
			this->stillSubDirCLabel->Size = System::Drawing::Size(66, 13);
			this->stillSubDirCLabel->TabIndex = 25;
			this->stillSubDirCLabel->Text = L"Subdirectory";
			// 
			// jpegExtensionLabel2
			// 
			this->jpegExtensionLabel2->AutoSize = true;
			this->jpegExtensionLabel2->Location = System::Drawing::Point(387, 70);
			this->jpegExtensionLabel2->Name = L"jpegExtensionLabel2";
			this->jpegExtensionLabel2->Size = System::Drawing::Size(33, 13);
			this->jpegExtensionLabel2->TabIndex = 24;
			this->jpegExtensionLabel2->Text = L"+ .jpg";
			// 
			// stillFileNameComboBox
			// 
			this->stillFileNameComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->stillFileNameComboBox->FormattingEnabled = true;
			this->stillFileNameComboBox->Location = System::Drawing::Point(238, 66);
			this->stillFileNameComboBox->Name = L"stillFileNameComboBox";
			this->stillFileNameComboBox->Size = System::Drawing::Size(143, 21);
			this->stillFileNameComboBox->TabIndex = 21;
			this->stillFileNameComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::stillFileNameComboBox_SelectedIndexChanged);
			// 
			// plusLabel4
			// 
			this->plusLabel4->AutoSize = true;
			this->plusLabel4->Location = System::Drawing::Point(214, 70);
			this->plusLabel4->Name = L"plusLabel4";
			this->plusLabel4->Size = System::Drawing::Size(13, 13);
			this->plusLabel4->TabIndex = 20;
			this->plusLabel4->Text = L"+";
			// 
			// stillFileNameTextBox
			// 
			this->stillFileNameTextBox->Location = System::Drawing::Point(79, 66);
			this->stillFileNameTextBox->Name = L"stillFileNameTextBox";
			this->stillFileNameTextBox->Size = System::Drawing::Size(123, 20);
			this->stillFileNameTextBox->TabIndex = 19;
			this->stillFileNameTextBox->TextChanged += gcnew System::EventHandler(this, &Settings::stillFileNameTextBox_TextChanged);
			// 
			// stillFileNameLabel
			// 
			this->stillFileNameLabel->AutoSize = true;
			this->stillFileNameLabel->Location = System::Drawing::Point(5, 68);
			this->stillFileNameLabel->Name = L"stillFileNameLabel";
			this->stillFileNameLabel->Size = System::Drawing::Size(52, 13);
			this->stillFileNameLabel->TabIndex = 18;
			this->stillFileNameLabel->Text = L"File name";
			// 
			// Recordingtab
			// 
			this->Recordingtab->Controls->Add(this->videoFileGroupBox);
			this->Recordingtab->Controls->Add(this->videoCompressionGroupBox);
			this->Recordingtab->Location = System::Drawing::Point(4, 22);
			this->Recordingtab->Name = L"Recordingtab";
			this->Recordingtab->Padding = System::Windows::Forms::Padding(3);
			this->Recordingtab->Size = System::Drawing::Size(442, 322);
			this->Recordingtab->TabIndex = 2;
			this->Recordingtab->Text = L"Recording";
			this->Recordingtab->UseVisualStyleBackColor = true;
			// 
			// videoFileGroupBox
			// 
			this->videoFileGroupBox->Controls->Add(this->exampleVideoFileNameLabel);
			this->videoFileGroupBox->Controls->Add(this->label3);
			this->videoFileGroupBox->Controls->Add(this->videoSubdirComboBox);
			this->videoFileGroupBox->Controls->Add(this->plusLabel6);
			this->videoFileGroupBox->Controls->Add(this->videoSubdirBox);
			this->videoFileGroupBox->Controls->Add(this->videoSubdirLabel);
			this->videoFileGroupBox->Controls->Add(this->schedulingButton);
			this->videoFileGroupBox->Controls->Add(this->driveAvailableLabel);
			this->videoFileGroupBox->Controls->Add(this->driveCapacityLabel);
			this->videoFileGroupBox->Controls->Add(this->driveNameLabel);
			this->videoFileGroupBox->Controls->Add(this->videoFileMaxDuration);
			this->videoFileGroupBox->Controls->Add(this->limitVideoFileDurationcheckBox);
			this->videoFileGroupBox->Controls->Add(this->videoFileNameComboBox);
			this->videoFileGroupBox->Controls->Add(this->plusLabel3);
			this->videoFileGroupBox->Controls->Add(this->aviExtensionLabel);
			this->videoFileGroupBox->Controls->Add(this->browseButton);
			this->videoFileGroupBox->Controls->Add(this->videoFolderTextBox);
			this->videoFileGroupBox->Controls->Add(this->videoDirectoryLabel);
			this->videoFileGroupBox->Controls->Add(this->videoFilenameBox);
			this->videoFileGroupBox->Controls->Add(this->videoFileNameLabel);
			this->videoFileGroupBox->Location = System::Drawing::Point(6, 6);
			this->videoFileGroupBox->Name = L"videoFileGroupBox";
			this->videoFileGroupBox->Size = System::Drawing::Size(430, 190);
			this->videoFileGroupBox->TabIndex = 2;
			this->videoFileGroupBox->TabStop = false;
			this->videoFileGroupBox->Text = L"Video file";
			// 
			// exampleVideoFileNameLabel
			// 
			this->exampleVideoFileNameLabel->AutoSize = true;
			this->exampleVideoFileNameLabel->Location = System::Drawing::Point(150, 106);
			this->exampleVideoFileNameLabel->Name = L"exampleVideoFileNameLabel";
			this->exampleVideoFileNameLabel->Size = System::Drawing::Size(0, 13);
			this->exampleVideoFileNameLabel->TabIndex = 33;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(93, 106);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(50, 13);
			this->label3->TabIndex = 32;
			this->label3->Text = L"Example:";
			// 
			// videoSubdirComboBox
			// 
			this->videoSubdirComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->videoSubdirComboBox->FormattingEnabled = true;
			this->videoSubdirComboBox->Location = System::Drawing::Point(254, 77);
			this->videoSubdirComboBox->Name = L"videoSubdirComboBox";
			this->videoSubdirComboBox->Size = System::Drawing::Size(143, 21);
			this->videoSubdirComboBox->TabIndex = 15;
			this->videoSubdirComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::videoSubdirComboBox_SelectedIndexChanged);
			// 
			// plusLabel6
			// 
			this->plusLabel6->AutoSize = true;
			this->plusLabel6->Location = System::Drawing::Point(238, 81);
			this->plusLabel6->Name = L"plusLabel6";
			this->plusLabel6->Size = System::Drawing::Size(13, 13);
			this->plusLabel6->TabIndex = 16;
			this->plusLabel6->Text = L"+";
			// 
			// videoSubdirBox
			// 
			this->videoSubdirBox->Location = System::Drawing::Point(95, 77);
			this->videoSubdirBox->Name = L"videoSubdirBox";
			this->videoSubdirBox->Size = System::Drawing::Size(140, 20);
			this->videoSubdirBox->TabIndex = 14;
			this->videoSubdirBox->TextChanged += gcnew System::EventHandler(this, &Settings::videoSubdirBox_TextChanged);
			// 
			// videoSubdirLabel
			// 
			this->videoSubdirLabel->AutoSize = true;
			this->videoSubdirLabel->Location = System::Drawing::Point(7, 80);
			this->videoSubdirLabel->Name = L"videoSubdirLabel";
			this->videoSubdirLabel->Size = System::Drawing::Size(66, 13);
			this->videoSubdirLabel->TabIndex = 13;
			this->videoSubdirLabel->Text = L"Subdirectory";
			// 
			// schedulingButton
			// 
			this->schedulingButton->Location = System::Drawing::Point(9, 158);
			this->schedulingButton->Name = L"schedulingButton";
			this->schedulingButton->Size = System::Drawing::Size(75, 23);
			this->schedulingButton->TabIndex = 12;
			this->schedulingButton->Text = L"Scheduler";
			this->schedulingButton->UseVisualStyleBackColor = true;
			this->schedulingButton->Click += gcnew System::EventHandler(this, &Settings::schedulingButton_Click);
			// 
			// driveAvailableLabel
			// 
			this->driveAvailableLabel->Location = System::Drawing::Point(301, 148);
			this->driveAvailableLabel->Name = L"driveAvailableLabel";
			this->driveAvailableLabel->Size = System::Drawing::Size(123, 23);
			this->driveAvailableLabel->TabIndex = 11;
			this->driveAvailableLabel->Text = L"Available: 0 GB";
			// 
			// driveCapacityLabel
			// 
			this->driveCapacityLabel->Location = System::Drawing::Point(300, 133);
			this->driveCapacityLabel->Name = L"driveCapacityLabel";
			this->driveCapacityLabel->Size = System::Drawing::Size(124, 23);
			this->driveCapacityLabel->TabIndex = 10;
			this->driveCapacityLabel->Text = L"Capacity : 0 GB";
			// 
			// driveNameLabel
			// 
			this->driveNameLabel->AutoSize = true;
			this->driveNameLabel->Location = System::Drawing::Point(251, 133);
			this->driveNameLabel->Name = L"driveNameLabel";
			this->driveNameLabel->Size = System::Drawing::Size(45, 13);
			this->driveNameLabel->TabIndex = 9;
			this->driveNameLabel->Text = L"Drive C:";
			// 
			// videoFileMaxDuration
			// 
			this->videoFileMaxDuration->CustomFormat = L"HH\'h\' mm\'min\'";
			this->videoFileMaxDuration->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->videoFileMaxDuration->Location = System::Drawing::Point(157, 129);
			this->videoFileMaxDuration->Name = L"videoFileMaxDuration";
			this->videoFileMaxDuration->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->videoFileMaxDuration->ShowUpDown = true;
			this->videoFileMaxDuration->Size = System::Drawing::Size(84, 20);
			this->videoFileMaxDuration->TabIndex = 8;
			// 
			// limitVideoFileDurationcheckBox
			// 
			this->limitVideoFileDurationcheckBox->AutoSize = true;
			this->limitVideoFileDurationcheckBox->Location = System::Drawing::Point(10, 132);
			this->limitVideoFileDurationcheckBox->Name = L"limitVideoFileDurationcheckBox";
			this->limitVideoFileDurationcheckBox->Size = System::Drawing::Size(148, 17);
			this->limitVideoFileDurationcheckBox->TabIndex = 7;
			this->limitVideoFileDurationcheckBox->Text = L"Limit video file duration to:";
			this->limitVideoFileDurationcheckBox->UseVisualStyleBackColor = true;
			this->limitVideoFileDurationcheckBox->CheckedChanged += gcnew System::EventHandler(this, &Settings::limitVideoFileDurationcheckBox_CheckedChanged);
			// 
			// videoFileNameComboBox
			// 
			this->videoFileNameComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->videoFileNameComboBox->FormattingEnabled = true;
			this->videoFileNameComboBox->Location = System::Drawing::Point(254, 49);
			this->videoFileNameComboBox->Name = L"videoFileNameComboBox";
			this->videoFileNameComboBox->Size = System::Drawing::Size(143, 21);
			this->videoFileNameComboBox->TabIndex = 3;
			this->videoFileNameComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::videoFileNameComboBox_SelectedIndexChanged);
			// 
			// plusLabel3
			// 
			this->plusLabel3->AutoSize = true;
			this->plusLabel3->Location = System::Drawing::Point(238, 53);
			this->plusLabel3->Name = L"plusLabel3";
			this->plusLabel3->Size = System::Drawing::Size(13, 13);
			this->plusLabel3->TabIndex = 6;
			this->plusLabel3->Text = L"+";
			// 
			// aviExtensionLabel
			// 
			this->aviExtensionLabel->AutoSize = true;
			this->aviExtensionLabel->Location = System::Drawing::Point(400, 53);
			this->aviExtensionLabel->Name = L"aviExtensionLabel";
			this->aviExtensionLabel->Size = System::Drawing::Size(24, 13);
			this->aviExtensionLabel->TabIndex = 5;
			this->aviExtensionLabel->Text = L".avi";
			// 
			// browseButton
			// 
			this->browseButton->Location = System::Drawing::Point(400, 17);
			this->browseButton->Name = L"browseButton";
			this->browseButton->Size = System::Drawing::Size(24, 22);
			this->browseButton->TabIndex = 1;
			this->browseButton->Text = L"...";
			this->browseButton->UseVisualStyleBackColor = true;
			this->browseButton->Click += gcnew System::EventHandler(this, &Settings::browseButton_Click);
			// 
			// videoFolderTextBox
			// 
			this->videoFolderTextBox->Location = System::Drawing::Point(95, 18);
			this->videoFolderTextBox->Name = L"videoFolderTextBox";
			this->videoFolderTextBox->Size = System::Drawing::Size(302, 20);
			this->videoFolderTextBox->TabIndex = 0;
			this->videoFolderTextBox->TextChanged += gcnew System::EventHandler(this, &Settings::videoFolderTextBox_TextChanged);
			this->videoFolderTextBox->Leave += gcnew System::EventHandler(this, &Settings::videoFolderTextBox_Leave);
			// 
			// videoDirectoryLabel
			// 
			this->videoDirectoryLabel->AutoSize = true;
			this->videoDirectoryLabel->Location = System::Drawing::Point(6, 22);
			this->videoDirectoryLabel->Name = L"videoDirectoryLabel";
			this->videoDirectoryLabel->Size = System::Drawing::Size(77, 13);
			this->videoDirectoryLabel->TabIndex = 2;
			this->videoDirectoryLabel->Text = L"Video directory";
			// 
			// videoFilenameBox
			// 
			this->videoFilenameBox->Location = System::Drawing::Point(95, 49);
			this->videoFilenameBox->Name = L"videoFilenameBox";
			this->videoFilenameBox->Size = System::Drawing::Size(140, 20);
			this->videoFilenameBox->TabIndex = 2;
			this->videoFilenameBox->TextChanged += gcnew System::EventHandler(this, &Settings::videoFilenameBox_TextChanged);
			// 
			// videoFileNameLabel
			// 
			this->videoFileNameLabel->AutoSize = true;
			this->videoFileNameLabel->Location = System::Drawing::Point(7, 52);
			this->videoFileNameLabel->Name = L"videoFileNameLabel";
			this->videoFileNameLabel->Size = System::Drawing::Size(52, 13);
			this->videoFileNameLabel->TabIndex = 0;
			this->videoFileNameLabel->Text = L"File name";
			// 
			// videoCompressionGroupBox
			// 
			this->videoCompressionGroupBox->Controls->Add(this->videoCodecPropPagesButton);
			this->videoCompressionGroupBox->Controls->Add(this->videoCodecsComboBox);
			this->videoCompressionGroupBox->Controls->Add(this->videoCodecsLabel);
			this->videoCompressionGroupBox->Location = System::Drawing::Point(6, 211);
			this->videoCompressionGroupBox->Name = L"videoCompressionGroupBox";
			this->videoCompressionGroupBox->Size = System::Drawing::Size(430, 103);
			this->videoCompressionGroupBox->TabIndex = 0;
			this->videoCompressionGroupBox->TabStop = false;
			this->videoCompressionGroupBox->Text = L"Video compression";
			// 
			// videoCodecPropPagesButton
			// 
			this->videoCodecPropPagesButton->Location = System::Drawing::Point(57, 48);
			this->videoCodecPropPagesButton->Name = L"videoCodecPropPagesButton";
			this->videoCodecPropPagesButton->Size = System::Drawing::Size(75, 23);
			this->videoCodecPropPagesButton->TabIndex = 1;
			this->videoCodecPropPagesButton->Text = L"Settings";
			this->videoCodecPropPagesButton->UseVisualStyleBackColor = true;
			this->videoCodecPropPagesButton->Click += gcnew System::EventHandler(this, &Settings::videoCodecPropPagesButton_Click);
			// 
			// videoCodecsComboBox
			// 
			this->videoCodecsComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->videoCodecsComboBox->FormattingEnabled = true;
			this->videoCodecsComboBox->Location = System::Drawing::Point(57, 20);
			this->videoCodecsComboBox->Name = L"videoCodecsComboBox";
			this->videoCodecsComboBox->Size = System::Drawing::Size(367, 21);
			this->videoCodecsComboBox->TabIndex = 0;
			this->videoCodecsComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::videoCodecsComboBox_SelectedIndexChanged);
			// 
			// videoCodecsLabel
			// 
			this->videoCodecsLabel->AutoSize = true;
			this->videoCodecsLabel->Location = System::Drawing::Point(7, 23);
			this->videoCodecsLabel->Name = L"videoCodecsLabel";
			this->videoCodecsLabel->Size = System::Drawing::Size(43, 13);
			this->videoCodecsLabel->TabIndex = 0;
			this->videoCodecsLabel->Text = L"Codecs";
			// 
			// WebServerTab
			// 
			this->WebServerTab->Location = System::Drawing::Point(4, 22);
			this->WebServerTab->Name = L"WebServerTab";
			this->WebServerTab->Padding = System::Windows::Forms::Padding(3);
			this->WebServerTab->Size = System::Drawing::Size(442, 322);
			this->WebServerTab->TabIndex = 4;
			this->WebServerTab->Text = L"Web server";
			this->WebServerTab->UseVisualStyleBackColor = true;
			// 
			// MotionDetectionTab
			// 
			this->MotionDetectionTab->Controls->Add(this->motionDetectionGroupBox);
			this->MotionDetectionTab->Location = System::Drawing::Point(4, 22);
			this->MotionDetectionTab->Name = L"MotionDetectionTab";
			this->MotionDetectionTab->Padding = System::Windows::Forms::Padding(3);
			this->MotionDetectionTab->Size = System::Drawing::Size(442, 322);
			this->MotionDetectionTab->TabIndex = 5;
			this->MotionDetectionTab->Text = L"Motion detection";
			this->MotionDetectionTab->UseVisualStyleBackColor = true;
			// 
			// motionDetectionGroupBox
			// 
			this->motionDetectionGroupBox->Location = System::Drawing::Point(6, 6);
			this->motionDetectionGroupBox->Name = L"motionDetectionGroupBox";
			this->motionDetectionGroupBox->Size = System::Drawing::Size(430, 312);
			this->motionDetectionGroupBox->TabIndex = 0;
			this->motionDetectionGroupBox->TabStop = false;
			this->motionDetectionGroupBox->Text = L"Motion detection settings";
			// 
			// FTPEnableCheckBox
			// 
			this->FTPEnableCheckBox->AutoSize = true;
			this->FTPEnableCheckBox->Location = System::Drawing::Point(12, 12);
			this->FTPEnableCheckBox->Name = L"FTPEnableCheckBox";
			this->FTPEnableCheckBox->Size = System::Drawing::Size(117, 17);
			this->FTPEnableCheckBox->TabIndex = 0;
			this->FTPEnableCheckBox->Text = L"Enable FTP upload";
			this->FTPEnableCheckBox->UseVisualStyleBackColor = true;
			this->FTPEnableCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Settings::FTPEnableCheckBox_CheckedChanged);
			// 
			// stillCaptureEnabledCheckBox
			// 
			this->stillCaptureEnabledCheckBox->AutoSize = true;
			this->stillCaptureEnabledCheckBox->Location = System::Drawing::Point(140, 12);
			this->stillCaptureEnabledCheckBox->Name = L"stillCaptureEnabledCheckBox";
			this->stillCaptureEnabledCheckBox->Size = System::Drawing::Size(151, 17);
			this->stillCaptureEnabledCheckBox->TabIndex = 0;
			this->stillCaptureEnabledCheckBox->Text = L"Enable still images capture";
			this->stillCaptureEnabledCheckBox->UseVisualStyleBackColor = true;
			this->stillCaptureEnabledCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Settings::stillCaptureEnabledCheckBox_CheckedChanged);
			// 
			// recordingEnabledCheckBox
			// 
			this->recordingEnabledCheckBox->AutoSize = true;
			this->recordingEnabledCheckBox->Location = System::Drawing::Point(302, 12);
			this->recordingEnabledCheckBox->Name = L"recordingEnabledCheckBox";
			this->recordingEnabledCheckBox->Size = System::Drawing::Size(135, 17);
			this->recordingEnabledCheckBox->TabIndex = 0;
			this->recordingEnabledCheckBox->Text = L"Enable video recording";
			this->recordingEnabledCheckBox->UseVisualStyleBackColor = true;
			this->recordingEnabledCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Settings::recordingEnabledCheckBox_CheckedChanged);
			// 
			// settingsCancelButton
			// 
			this->settingsCancelButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->settingsCancelButton->Location = System::Drawing::Point(385, 412);
			this->settingsCancelButton->Name = L"settingsCancelButton";
			this->settingsCancelButton->Size = System::Drawing::Size(75, 23);
			this->settingsCancelButton->TabIndex = 1;
			this->settingsCancelButton->Text = L"Cancel";
			this->settingsCancelButton->UseVisualStyleBackColor = true;
			this->settingsCancelButton->Click += gcnew System::EventHandler(this, &Settings::settingsCancelButton_Click);
			// 
			// settingsOKButton
			// 
			this->settingsOKButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->settingsOKButton->Location = System::Drawing::Point(304, 412);
			this->settingsOKButton->Name = L"settingsOKButton";
			this->settingsOKButton->Size = System::Drawing::Size(75, 23);
			this->settingsOKButton->TabIndex = 2;
			this->settingsOKButton->Text = L"OK";
			this->settingsOKButton->UseVisualStyleBackColor = true;
			this->settingsOKButton->Click += gcnew System::EventHandler(this, &Settings::settingsOKButton_Click);
			// 
			// videoDirectoryErrorProvider
			// 
			this->videoDirectoryErrorProvider->ContainerControl = this;
			// 
			// settingsApplyButton
			// 
			this->settingsApplyButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->settingsApplyButton->Enabled = false;
			this->settingsApplyButton->Location = System::Drawing::Point(223, 412);
			this->settingsApplyButton->Name = L"settingsApplyButton";
			this->settingsApplyButton->Size = System::Drawing::Size(75, 23);
			this->settingsApplyButton->TabIndex = 3;
			this->settingsApplyButton->Text = L"Apply";
			this->settingsApplyButton->UseVisualStyleBackColor = true;
			this->settingsApplyButton->Click += gcnew System::EventHandler(this, &Settings::settingsApplyButton_Click);
			// 
			// webServerEnableCheckBox
			// 
			this->webServerEnableCheckBox->AutoSize = true;
			this->webServerEnableCheckBox->Location = System::Drawing::Point(12, 35);
			this->webServerEnableCheckBox->Name = L"webServerEnableCheckBox";
			this->webServerEnableCheckBox->Size = System::Drawing::Size(119, 17);
			this->webServerEnableCheckBox->TabIndex = 4;
			this->webServerEnableCheckBox->Text = L"Enable Web Server";
			this->webServerEnableCheckBox->UseVisualStyleBackColor = true;
			this->webServerEnableCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Settings::webServerEnableCheckBox_CheckedChanged);

			this->webServerEnableCheckBox->Enabled = false;

			// 
			// motionDetectionEnableCheckBox
			// 
			this->motionDetectionEnableCheckBox->AutoSize = true;
			this->motionDetectionEnableCheckBox->Location = System::Drawing::Point(140, 35);
			this->motionDetectionEnableCheckBox->Name = L"motionDetectionEnableCheckBox";
			this->motionDetectionEnableCheckBox->Size = System::Drawing::Size(140, 17);
			this->motionDetectionEnableCheckBox->TabIndex = 5;
			this->motionDetectionEnableCheckBox->Text = L"Enable motion detection";
			this->motionDetectionEnableCheckBox->UseVisualStyleBackColor = true;
			this->motionDetectionEnableCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Settings::motionDetectionEnableCheckBox_CheckedChanged);
			// 
			// Settings
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(474, 443);
			this->Controls->Add(this->motionDetectionEnableCheckBox);
			this->Controls->Add(this->webServerEnableCheckBox);
			this->Controls->Add(this->FTPEnableCheckBox);
			this->Controls->Add(this->recordingEnabledCheckBox);
			this->Controls->Add(this->stillCaptureEnabledCheckBox);
			this->Controls->Add(this->settingsApplyButton);
			this->Controls->Add(this->settingsOKButton);
			this->Controls->Add(this->settingsCancelButton);
			this->Controls->Add(this->tabControl1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Settings";
			this->Text = L"Settings";
			this->tabControl1->ResumeLayout(false);
			this->CapDevicesTab->ResumeLayout(false);
			this->audioDevicesGroupBox->ResumeLayout(false);
			this->audioDevicesGroupBox->PerformLayout();
			this->videoDevicesGroupBox->ResumeLayout(false);
			this->videoDevicesGroupBox->PerformLayout();
			this->FTPtab->ResumeLayout(false);
			this->FTPMiscGroupBox->ResumeLayout(false);
			this->FTPMiscGroupBox->PerformLayout();
			this->FTPServerDetailsGroupBox->ResumeLayout(false);
			this->FTPServerDetailsGroupBox->PerformLayout();
			this->StillImagestab->ResumeLayout(false);
			this->StillImgDetailsGroupBox->ResumeLayout(false);
			this->StillImgDetailsGroupBox->PerformLayout();
			this->Recordingtab->ResumeLayout(false);
			this->videoFileGroupBox->ResumeLayout(false);
			this->videoFileGroupBox->PerformLayout();
			this->videoCompressionGroupBox->ResumeLayout(false);
			this->videoCompressionGroupBox->PerformLayout();
			this->MotionDetectionTab->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->videoDirectoryErrorProvider))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void jpegQualityTextBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
			 // Accept only digits and the Backspace character
			 if(!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08)
				 e->Handled = true;
		 }
private: System::Void jpegQualityScrollBar_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {
			 char cQuality[4];
			 _itoa_s(this->jpegQualityScrollBar->Value, cQuality, 4, 10);
			 String ^sQuality= gcnew String(cQuality);
			 this->jpegQualityTextBox->Text = sQuality;
		 }
private: System::Void jpegQualityTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 String ^sQuality= gcnew String(this->jpegQualityTextBox->Text);
			 int quality = 0;
			 if(sQuality != "")
				 quality = Int32::Parse(sQuality);

			 if(quality < 0)
				 quality = 0;
			 if(quality > 100)
				 quality = 100;

			 this->jpegQualityScrollBar->Value = quality;
			 char cQuality[4];
			 sprintf_s(cQuality, "%d", quality);
			 this->jpegQualityTextBox->Text = gcnew String(cQuality);
		 }
private: System::Void settingsCancelButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void settingsOKButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(!this->videoDirectoryErrorProvider->GetError(this->videoFolderTextBox)->Equals(""))
			 {
				 MessageBox::Show("Please enter a valid video directory", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				 this->tabControl1->SelectedIndex = 2;
				 return;
			 }

			if(this->videoFileNameComboBox->SelectedIndex == 0 && this->limitVideoFileDurationcheckBox->Checked)
			{
				String^ message = "Warning, you've chosen no extension to the video file and a limited duration of this file.\nIt means that the new file will overwrite the previous one after each time slice.\nAre you sure that it's what you want?";
				String^ caption = "Warning";
				System::Windows::Forms::DialogResult res = MessageBox::Show(message, caption, MessageBoxButtons::YesNo, MessageBoxIcon::Warning);
				if(res == System::Windows::Forms::DialogResult::No)
					return;
			}

			 // Apply settings
			 ApplySettings();

			 this->DialogResult = m_dialogResult;
			 // Then close
			 this->Close();
		 }
private: System::Void settingsApplyButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(!this->videoDirectoryErrorProvider->GetError(this->videoFolderTextBox)->Equals(""))
			 {
				 MessageBox::Show("Please enter a valid video directory", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				 this->tabControl1->SelectedIndex = 2;
				 return;
			 }

			if(this->videoFileNameComboBox->SelectedIndex == 0 && this->limitVideoFileDurationcheckBox->Checked)
			{
				String^ message = "Warning, you've chosen no extension to the video file and a limited duration of this file.\nIt means that the new file will overwrite the previous one after each time slice.\nAre you sure that it's what you want?";
				String^ caption = "Warning";
				System::Windows::Forms::DialogResult res = MessageBox::Show(message, caption, MessageBoxButtons::YesNo, MessageBoxIcon::Warning);
				if(res == System::Windows::Forms::DialogResult::No)
					return;
			}

			 // Apply settings
			 ApplySettings();
			 this->settingsApplyButton->Enabled = false;
			 this->videoCodecPropPagesButton->Enabled = true;
			 //this->CodecSettingsToolTip->Hide(this->videoCodecPropPagesButton);
		 }
private: System::Void imageFileNameComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 RefreshImageFileName();
		 }
private: System::Void imageFileNameBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 RefreshImageFileName();
		 }
private: System::Void FTPEnableCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 bool val = this->FTPEnableCheckBox->Checked::get();
			 if(val)
				 EnableFTPSettings(true);
			 else
				 EnableFTPSettings(false);
		 }
private: System::Void imageSubDirBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 RefreshImageFileName();
		 }
private: System::Void imageSubDirComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 RefreshImageFileName();
		 }
private: System::Void recordingEnabledCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 if(this->recordingEnabledCheckBox->Checked)
				 EnableRecordingSettings(true);
			 else
				 EnableRecordingSettings(false);
		 }
private: System::Void browseButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->videoFolderBrowserDialog->ShowDialog();
			 this->videoFolderTextBox->Text = videoFolderBrowserDialog->SelectedPath;
			 UpdateDriveInfo(this->videoFolderTextBox->Text);
		 }
private: System::Void videoFolderTextBox_Leave(System::Object^  sender, System::EventArgs^  e) {
			 if(!System::IO::Directory::Exists(this->videoFolderTextBox->Text))
			 {
				 this->videoDirectoryErrorProvider->SetIconAlignment(this->videoFolderTextBox, ErrorIconAlignment::MiddleLeft);
				 this->videoDirectoryErrorProvider->SetError(this->videoFolderTextBox, gcnew String("Directory doesn't exist"));
			 }
			 else
			 {
				 this->videoDirectoryErrorProvider->SetError(this->videoFolderTextBox, "" );
				 UpdateDriveInfo(this->videoFolderTextBox->Text);
			 }
		 }
private: System::Void videoFolderTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 if(System::IO::Directory::Exists(this->videoFolderTextBox->Text))
				 this->videoDirectoryErrorProvider->SetError(this->videoFolderTextBox, "" );
		 }
private: System::Void videoCapturePropButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 //videoCapturePropButton
			 m_pGraph->DisplayVideoDevicePropPages();
		 }
private: System::Void videoCodecPropPagesButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 //videoCodecPropPagesButton
			 m_pGraph->DisplayVideoCompressorPropPages();
		 }
private: System::Void audCapDeviceComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
		 }
private: System::Void vidCapDeviceComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
		 }
private: System::Void videoFilenameBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 RefreshVideoImageFileName();
		 }
private: System::Void videoFileNameComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 RefreshVideoImageFileName();
		 }
private: System::Void videoCodecsComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 this->videoCodecPropPagesButton->Enabled = false;
			 //this->CodecSettingsToolTip->Show("You must click on \"Apply\" to enable this button",this->videoCodecPropPagesButton);
		 }
private: System::Void serverNameBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
		 }
private: System::Void userNameBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
		 }
private: System::Void passwordBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
		 }
private: System::Void directoryBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
		 }
private: System::Void imageSizeComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
		 }
private: System::Void uploadPeriodTimePicker_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
		 }
private: System::Void limitVideoFileDurationcheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 this->videoFileMaxDuration->Enabled = this->limitVideoFileDurationcheckBox->Checked;
		 }
private: System::Void schedulingButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 SchedulingForm^ newMDIChild = gcnew SchedulingForm(m_pGraph);
				 newMDIChild->ShowDialog();
		 }
private: System::Void stillCaptureEnabledCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 bool val = this->stillCaptureEnabledCheckBox->Checked::get();
			 this->StillImgDetailsGroupBox->Enabled::set(val?true:false);
		 }
private: System::Void stillFolderButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->stillFolderBrowserDialog->ShowDialog();
			 this->stillFolderTextBox->Text = stillFolderBrowserDialog->SelectedPath;			 
		 }
private: System::Void stillPeriodDateTimePicker_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
		 }
private: System::Void stillFileNameTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
 			 this->settingsApplyButton->Enabled = true;
			 RefreshStillImageFileName();
		 }
private: System::Void stillFileNameComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
 			 this->settingsApplyButton->Enabled = true;
			 RefreshStillImageFileName();
		 }
private: System::Void stillSubDirTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
 			 this->settingsApplyButton->Enabled = true;
			 RefreshStillImageFileName();
		 }
private: System::Void stillSubDirComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
 			 this->settingsApplyButton->Enabled = true;
			 RefreshStillImageFileName();
		 }
private: System::Void stillJpegQualityScrollBar_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {
			 char cQuality[4];
			 _itoa_s(this->stillJpegQualityScrollBar->Value, cQuality, 4, 10);
			 this->stillJpegQualityTextBox->Text = gcnew String(cQuality);
		 }
private: System::Void stillJpegQualityTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 String ^sQuality= gcnew String(this->stillJpegQualityTextBox->Text);
			 int quality = 0;
			 if(sQuality != "")
				 quality = Int32::Parse(sQuality);

			 if(quality < 0)
				 quality = 0;
			 if(quality > 100)
				 quality = 100;

			 this->stillJpegQualityScrollBar->Value = quality;
			 char cQuality[4];
			 sprintf_s(cQuality, "%d", quality);
			 this->stillJpegQualityTextBox->Text = gcnew String(cQuality);
		 }
private: System::Void stillImageSizeComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			this->settingsApplyButton->Enabled = true;
		 }
private: System::Void videoSubdirBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
 			 this->settingsApplyButton->Enabled = true;
			 RefreshVideoImageFileName();
		 }
private: System::Void videoSubdirComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
 			 this->settingsApplyButton->Enabled = true;
			 RefreshVideoImageFileName();
		 }
private: System::Void StillOverlayEnabledCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			this->settingsApplyButton->Enabled = true;
		 }
private: System::Void OverlayEnabledCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			this->settingsApplyButton->Enabled = true;
		 }
private: System::Void stillJpegQualityTextBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
			 // Accept only digits and the Backspace character
			 if(!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08)
				 e->Handled = true;
		 }
private: System::Void posXTextBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
			 // Accept only digits and the Backspace character
			 if(!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08)
				 e->Handled = true;
		 }
private: System::Void posYTextBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
			 // Accept only digits and the Backspace character
			 if(!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08)
				 e->Handled = true;
		 }
private: System::Void posXTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 String ^sPosX= gcnew String(this->posXTextBox->Text);
			 int posx = 0;
			 if(sPosX != "")
				 posx = Int32::Parse(sPosX);

			 if(posx < 0)
				 posx = 0;
			 if(posx>640)
				 posx = 640;

 			 char cPosX[4];
			 sprintf_s(cPosX, "%d", posx);
			 this->posXTextBox->Text = gcnew String(cPosX);
		 }
private: System::Void posYTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 String ^sPosY= gcnew String(this->posYTextBox->Text);
			 int posy = 0;
			 if(sPosY != "")
				 posy = Int32::Parse(sPosY);

			 if(posy < 0)
				 posy = 0;
			 if(posy>480)
				 posy = 480;

 			 char cPosY[4];
			 sprintf_s(cPosY, "%d", posy);
			 this->posYTextBox->Text = gcnew String(cPosY);		 }
private: System::Void webServerEnableCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(this->webServerEnableCheckBox->Checked)
				 m_pWebServer->Enable();
			 else
				 m_pWebServer->Disable();
		 }
private: System::Void motionDetectionEnableCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->settingsApplyButton->Enabled = true;
			 bool val = this->motionDetectionEnableCheckBox->Checked::get();
			 this->motionDetectionGroupBox->Enabled::set(val?true:false);
		 }
};
}
