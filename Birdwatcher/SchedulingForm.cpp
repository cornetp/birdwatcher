#include "StdAfx.h"
#include "SchedulingForm.h"
#include "Debug.h"

namespace Birdwatcher {

void SchedulingForm::InitCells()
{
	SYSTEMTIME startTime[7];
	SYSTEMTIME stopTime[7];
	for(int i=0; i<7; i++)
	{
		m_pGraph->GetVideoRecordingSchedule(i, &startTime[i], &stopTime[i]);
		m_startHour[i] = -1;//startTime.wHour;
		m_stopHour[i] = -1;//stopTime.wHour;
	}

	HourCheckBoxes = gcnew array<System::Windows::Forms::CheckBox^>(24);
	TimeSlots = gcnew array<System::Windows::Forms::CheckBox^>(24*7);
	for(int i = 0; i < 24; i++)
	{
		this->HourCheckBoxes[i] = (gcnew System::Windows::Forms::CheckBox());
		this->HourCheckBoxes[i]->Appearance = System::Windows::Forms::Appearance::Button;
		this->HourCheckBoxes[i]->BackColor = System::Drawing::SystemColors::Control;
		this->HourCheckBoxes[i]->FlatAppearance->BorderSize = 0;
		this->HourCheckBoxes[i]->FlatAppearance->CheckedBackColor = System::Drawing::SystemColors::Control;
		this->HourCheckBoxes[i]->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
		this->HourCheckBoxes[i]->Margin = System::Windows::Forms::Padding(0);
		this->HourCheckBoxes[i]->Name = L"HourCheckBox"+i;
		this->HourCheckBoxes[i]->Size = System::Drawing::Size(10, 20);
		//char txt[8];
		//_itoa_s(i, txt, 8, 10);
		//this->HourCheckBoxes[i]->Text = gcnew String(txt);
		this->HourCheckBoxes[i]->Text = "";
		this->HourCheckBoxes[i]->UseVisualStyleBackColor = false;
		this->HourCheckBoxes[i]->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		this->HourCheckBoxes[i]->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 4, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));

		this->HourCheckBoxes[i]->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::HourCheckBox_CheckedChanged);
		this->tableLayoutPanel1->Controls->Add(this->HourCheckBoxes[i], i+1, 0);

		for(int d=0; d<7; d++)
		{
			int ix = i+d*24;
			this->TimeSlots[ix] = (gcnew System::Windows::Forms::CheckBox());
			this->TimeSlots[ix]->Appearance = System::Windows::Forms::Appearance::Button;
			this->TimeSlots[ix]->AutoSize = false;
			this->TimeSlots[ix]->BackColor = System::Drawing::Color::White;
			this->TimeSlots[ix]->FlatAppearance->BorderSize = 0;
			this->TimeSlots[ix]->FlatAppearance->CheckedBackColor = System::Drawing::Color::Blue;
			this->TimeSlots[ix]->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Blue;
			this->TimeSlots[ix]->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TimeSlots[ix]->Margin = System::Windows::Forms::Padding(0);
			this->TimeSlots[ix]->Name = L"TimeSlot_"+i+"_"+d;
			this->TimeSlots[ix]->Size = System::Drawing::Size(10, 20);
			this->TimeSlots[ix]->Text = L"";
			this->TimeSlots[ix]->UseVisualStyleBackColor = true;

			this->TimeSlots[ix]->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::TimeSlotCheckBox_CheckedChanged);
			this->tableLayoutPanel1->Controls->Add(this->TimeSlots[ix], i+1, d+1);


			//if(i>=m_startHour[d] && i<m_stopHour[d])
			if(i>=startTime[d].wHour && i<stopTime[d].wHour)
				this->TimeSlots[ix]->Checked = true;
		}
	}

	this->enableSchedulingCheckBox->Checked = m_pGraph->GetEnableRecordingSchedule();
	EnableControls(this->enableSchedulingCheckBox->Checked);
}

void SchedulingForm::EnableControls(bool bEnable)
{/*
	if(bEnable)
		this->tableLayoutPanel1->Cursor = System::Windows::Forms::Cursors::Default;
	else
		this->tableLayoutPanel1->Cursor = System::Windows::Forms::Cursors::No;*/

	this->tableLayoutPanel1->Enabled = bEnable;
	this->_0Label->Enabled = bEnable;
	this->_4Label->Enabled = bEnable;
	this->_8Label->Enabled = bEnable;
	this->_12Label->Enabled = bEnable;
	this->_16Label->Enabled = bEnable;
	this->_20Label->Enabled = bEnable;
	this->_24Label->Enabled = bEnable;
}

void SchedulingForm::Apply()
{
	if(this->enableSchedulingCheckBox->Checked)
	{
		m_pGraph->SetEnableRecordingSchedule(true);
		SYSTEMTIME startTime;
		SYSTEMTIME stopTime;
		for(int i=0; i<7; i++)
		{
			startTime.wHour = m_startHour[i];
			startTime.wMinute = 0;
			startTime.wSecond = 0;

			stopTime.wHour = m_stopHour[i];
			stopTime.wMinute = 0;
			stopTime.wSecond = 0;

			m_pGraph->SetVideoRecordingSchedule(i, &startTime, &stopTime);
		}
	}
	else
		m_pGraph->SetEnableRecordingSchedule(false);
}

};
