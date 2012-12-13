#pragma once

#include "FilterGraph.h"
#include "Debug.h"

namespace Birdwatcher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for SchedulingForm
	/// </summary>
	public ref class SchedulingForm : public System::Windows::Forms::Form
	{
	public:
		SchedulingForm(CFilterGraph *pGraph)
		{
			m_startHour = gcnew array<int>(7);
			m_stopHour = gcnew array<int>(7);
			m_pGraph = pGraph;
			InitializeComponent();
			InitCells();
		}
	private:
		void InitCells();
		void Apply();
		void EnableControls(bool bEnable);
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SchedulingForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: CFilterGraph *m_pGraph;
	private: array<int>^m_startHour;
	private: array<int>^m_stopHour;
	private: array<System::Windows::Forms::CheckBox^>^HourCheckBoxes;
	private: array<System::Windows::Forms::CheckBox^>^TimeSlots;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::CheckBox^  SundayCheckBox;
	private: System::Windows::Forms::CheckBox^  MondayCheckBox;
	private: System::Windows::Forms::CheckBox^  TuesdayCheckBox;
	private: System::Windows::Forms::CheckBox^  WednesdayCheckBox;
	private: System::Windows::Forms::CheckBox^  ThursdayCheckBox;
	private: System::Windows::Forms::CheckBox^  FridayCheckBox;
	private: System::Windows::Forms::CheckBox^  SaterdayCheckBox;
	private: System::Windows::Forms::CheckBox^  enableSchedulingCheckBox;
	private: System::Windows::Forms::Button^  OKButton;
	private: System::Windows::Forms::Button^  cancelButton;
	private: System::Windows::Forms::Label^  _0Label;
	private: System::Windows::Forms::Label^  _24Label;
	private: System::Windows::Forms::Label^  _4Label;

	private: System::Windows::Forms::Label^  _8Label;
	private: System::Windows::Forms::Label^  _12Label;
	private: System::Windows::Forms::Label^  _16Label;
	private: System::Windows::Forms::Label^  _20Label;



	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->SundayCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->MondayCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->TuesdayCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->WednesdayCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->ThursdayCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->FridayCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->SaterdayCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->enableSchedulingCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->OKButton = (gcnew System::Windows::Forms::Button());
			this->cancelButton = (gcnew System::Windows::Forms::Button());
			this->_0Label = (gcnew System::Windows::Forms::Label());
			this->_24Label = (gcnew System::Windows::Forms::Label());
			this->_4Label = (gcnew System::Windows::Forms::Label());
			this->_8Label = (gcnew System::Windows::Forms::Label());
			this->_12Label = (gcnew System::Windows::Forms::Label());
			this->_16Label = (gcnew System::Windows::Forms::Label());
			this->_20Label = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel1->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;
			this->tableLayoutPanel1->ColumnCount = 25;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				100)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				20)));
			this->tableLayoutPanel1->Controls->Add(this->SundayCheckBox, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->MondayCheckBox, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->TuesdayCheckBox, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->WednesdayCheckBox, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->ThursdayCheckBox, 0, 5);
			this->tableLayoutPanel1->Controls->Add(this->FridayCheckBox, 0, 6);
			this->tableLayoutPanel1->Controls->Add(this->SaterdayCheckBox, 0, 7);
			this->tableLayoutPanel1->ForeColor = System::Drawing::SystemColors::ControlText;
			this->tableLayoutPanel1->Location = System::Drawing::Point(12, 58);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 8;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(366, 170);
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// SundayCheckBox
			// 
			this->SundayCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->SundayCheckBox->BackColor = System::Drawing::SystemColors::Control;
			this->SundayCheckBox->FlatAppearance->BorderSize = 0;
			this->SundayCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::SystemColors::Control;
			this->SundayCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->SundayCheckBox->Location = System::Drawing::Point(1, 22);
			this->SundayCheckBox->Margin = System::Windows::Forms::Padding(0);
			this->SundayCheckBox->Name = L"SundayCheckBox";
			this->SundayCheckBox->Size = System::Drawing::Size(100, 20);
			this->SundayCheckBox->TabIndex = 7;
			this->SundayCheckBox->Text = L"Sunday";
			this->SundayCheckBox->UseVisualStyleBackColor = false;
			this->SundayCheckBox->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::DayCheckBox_CheckedChanged);
			// 
			// MondayCheckBox
			// 
			this->MondayCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->MondayCheckBox->FlatAppearance->BorderSize = 0;
			this->MondayCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::SystemColors::Control;
			this->MondayCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->MondayCheckBox->Location = System::Drawing::Point(1, 43);
			this->MondayCheckBox->Margin = System::Windows::Forms::Padding(0);
			this->MondayCheckBox->Name = L"MondayCheckBox";
			this->MondayCheckBox->Size = System::Drawing::Size(100, 20);
			this->MondayCheckBox->TabIndex = 8;
			this->MondayCheckBox->Text = L"Monday";
			this->MondayCheckBox->UseVisualStyleBackColor = true;
			this->MondayCheckBox->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::DayCheckBox_CheckedChanged);
			// 
			// TuesdayCheckBox
			// 
			this->TuesdayCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->TuesdayCheckBox->FlatAppearance->BorderSize = 0;
			this->TuesdayCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::SystemColors::Control;
			this->TuesdayCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TuesdayCheckBox->Location = System::Drawing::Point(1, 64);
			this->TuesdayCheckBox->Margin = System::Windows::Forms::Padding(0);
			this->TuesdayCheckBox->Name = L"TuesdayCheckBox";
			this->TuesdayCheckBox->Size = System::Drawing::Size(100, 20);
			this->TuesdayCheckBox->TabIndex = 9;
			this->TuesdayCheckBox->Text = L"Tuesday";
			this->TuesdayCheckBox->UseVisualStyleBackColor = true;
			this->TuesdayCheckBox->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::DayCheckBox_CheckedChanged);
			// 
			// WednesdayCheckBox
			// 
			this->WednesdayCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->WednesdayCheckBox->FlatAppearance->BorderSize = 0;
			this->WednesdayCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::SystemColors::Control;
			this->WednesdayCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->WednesdayCheckBox->Location = System::Drawing::Point(1, 85);
			this->WednesdayCheckBox->Margin = System::Windows::Forms::Padding(0);
			this->WednesdayCheckBox->Name = L"WednesdayCheckBox";
			this->WednesdayCheckBox->Size = System::Drawing::Size(100, 20);
			this->WednesdayCheckBox->TabIndex = 10;
			this->WednesdayCheckBox->Text = L"Wednesday";
			this->WednesdayCheckBox->UseVisualStyleBackColor = true;
			this->WednesdayCheckBox->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::DayCheckBox_CheckedChanged);
			// 
			// ThursdayCheckBox
			// 
			this->ThursdayCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->ThursdayCheckBox->FlatAppearance->BorderSize = 0;
			this->ThursdayCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::SystemColors::Control;
			this->ThursdayCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ThursdayCheckBox->Location = System::Drawing::Point(1, 106);
			this->ThursdayCheckBox->Margin = System::Windows::Forms::Padding(0);
			this->ThursdayCheckBox->Name = L"ThursdayCheckBox";
			this->ThursdayCheckBox->Size = System::Drawing::Size(100, 20);
			this->ThursdayCheckBox->TabIndex = 11;
			this->ThursdayCheckBox->Text = L"Thursday";
			this->ThursdayCheckBox->UseVisualStyleBackColor = true;
			this->ThursdayCheckBox->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::DayCheckBox_CheckedChanged);
			// 
			// FridayCheckBox
			// 
			this->FridayCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->FridayCheckBox->FlatAppearance->BorderSize = 0;
			this->FridayCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::SystemColors::Control;
			this->FridayCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->FridayCheckBox->Location = System::Drawing::Point(1, 127);
			this->FridayCheckBox->Margin = System::Windows::Forms::Padding(0);
			this->FridayCheckBox->Name = L"FridayCheckBox";
			this->FridayCheckBox->Size = System::Drawing::Size(100, 20);
			this->FridayCheckBox->TabIndex = 12;
			this->FridayCheckBox->Text = L"Friday";
			this->FridayCheckBox->UseVisualStyleBackColor = true;
			this->FridayCheckBox->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::DayCheckBox_CheckedChanged);
			// 
			// SaterdayCheckBox
			// 
			this->SaterdayCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->SaterdayCheckBox->FlatAppearance->BorderSize = 0;
			this->SaterdayCheckBox->FlatAppearance->CheckedBackColor = System::Drawing::SystemColors::Control;
			this->SaterdayCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->SaterdayCheckBox->Location = System::Drawing::Point(1, 148);
			this->SaterdayCheckBox->Margin = System::Windows::Forms::Padding(0);
			this->SaterdayCheckBox->Name = L"SaterdayCheckBox";
			this->SaterdayCheckBox->Size = System::Drawing::Size(100, 20);
			this->SaterdayCheckBox->TabIndex = 13;
			this->SaterdayCheckBox->Text = L"Saterday";
			this->SaterdayCheckBox->UseVisualStyleBackColor = true;
			this->SaterdayCheckBox->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::DayCheckBox_CheckedChanged);
			// 
			// enableSchedulingCheckBox
			// 
			this->enableSchedulingCheckBox->AutoSize = true;
			this->enableSchedulingCheckBox->Location = System::Drawing::Point(13, 13);
			this->enableSchedulingCheckBox->Name = L"enableSchedulingCheckBox";
			this->enableSchedulingCheckBox->Size = System::Drawing::Size(113, 17);
			this->enableSchedulingCheckBox->TabIndex = 1;
			this->enableSchedulingCheckBox->Text = L"Enable scheduling";
			this->enableSchedulingCheckBox->UseVisualStyleBackColor = true;
			this->enableSchedulingCheckBox->CheckedChanged += gcnew System::EventHandler(this, &SchedulingForm::enableSchedulingCheckBox_CheckedChanged);
			// 
			// OKButton
			// 
			this->OKButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->OKButton->Location = System::Drawing::Point(220, 236);
			this->OKButton->Name = L"OKButton";
			this->OKButton->Size = System::Drawing::Size(75, 23);
			this->OKButton->TabIndex = 2;
			this->OKButton->Text = L"OK";
			this->OKButton->UseVisualStyleBackColor = true;
			this->OKButton->Click += gcnew System::EventHandler(this, &SchedulingForm::OKButton_Click);
			// 
			// cancelButton
			// 
			this->cancelButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->cancelButton->Location = System::Drawing::Point(303, 236);
			this->cancelButton->Name = L"cancelButton";
			this->cancelButton->Size = System::Drawing::Size(75, 23);
			this->cancelButton->TabIndex = 3;
			this->cancelButton->Text = L"Cancel";
			this->cancelButton->UseVisualStyleBackColor = true;
			this->cancelButton->Click += gcnew System::EventHandler(this, &SchedulingForm::cancelButton_Click);
			// 
			// _0Label
			// 
			this->_0Label->AutoSize = true;
			this->_0Label->Location = System::Drawing::Point(107, 45);
			this->_0Label->Name = L"_0Label";
			this->_0Label->Size = System::Drawing::Size(13, 13);
			this->_0Label->TabIndex = 4;
			this->_0Label->Text = L"0";
			// 
			// _24Label
			// 
			this->_24Label->AutoSize = true;
			this->_24Label->Location = System::Drawing::Point(367, 45);
			this->_24Label->Name = L"_24Label";
			this->_24Label->Size = System::Drawing::Size(19, 13);
			this->_24Label->TabIndex = 5;
			this->_24Label->Text = L"24";
			// 
			// _4Label
			// 
			this->_4Label->AutoSize = true;
			this->_4Label->Location = System::Drawing::Point(151, 45);
			this->_4Label->Name = L"_4Label";
			this->_4Label->Size = System::Drawing::Size(13, 13);
			this->_4Label->TabIndex = 6;
			this->_4Label->Text = L"4";
			// 
			// _8Label
			// 
			this->_8Label->AutoSize = true;
			this->_8Label->Location = System::Drawing::Point(194, 45);
			this->_8Label->Name = L"_8Label";
			this->_8Label->Size = System::Drawing::Size(13, 13);
			this->_8Label->TabIndex = 7;
			this->_8Label->Text = L"8";
			// 
			// _12Label
			// 
			this->_12Label->AutoSize = true;
			this->_12Label->Location = System::Drawing::Point(236, 45);
			this->_12Label->Name = L"_12Label";
			this->_12Label->Size = System::Drawing::Size(19, 13);
			this->_12Label->TabIndex = 8;
			this->_12Label->Text = L"12";
			// 
			// _16Label
			// 
			this->_16Label->AutoSize = true;
			this->_16Label->Location = System::Drawing::Point(281, 45);
			this->_16Label->Name = L"_16Label";
			this->_16Label->Size = System::Drawing::Size(19, 13);
			this->_16Label->TabIndex = 9;
			this->_16Label->Text = L"16";
			// 
			// _20Label
			// 
			this->_20Label->AutoSize = true;
			this->_20Label->Location = System::Drawing::Point(324, 45);
			this->_20Label->Name = L"_20Label";
			this->_20Label->Size = System::Drawing::Size(19, 13);
			this->_20Label->TabIndex = 10;
			this->_20Label->Text = L"20";
			// 
			// SchedulingForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(390, 265);
			this->Controls->Add(this->_20Label);
			this->Controls->Add(this->_16Label);
			this->Controls->Add(this->_12Label);
			this->Controls->Add(this->_8Label);
			this->Controls->Add(this->_4Label);
			this->Controls->Add(this->_24Label);
			this->Controls->Add(this->_0Label);
			this->Controls->Add(this->cancelButton);
			this->Controls->Add(this->OKButton);
			this->Controls->Add(this->enableSchedulingCheckBox);
			this->Controls->Add(this->tableLayoutPanel1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"SchedulingForm";
			this->Text = L"SchedulingForm";
			this->tableLayoutPanel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void DayCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				System::Windows::Forms::CheckBox^ checkBox = (CheckBox^)sender;
				int row = this->tableLayoutPanel1->GetRow(checkBox);
				for(int i=0; i<24; i++)
				{
					int ix = i+(row-1)*24;
					this->TimeSlots[ix]->Checked = checkBox->Checked;
				}
		 }

private: System::Void HourCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				System::Windows::Forms::CheckBox^ checkBox = (CheckBox^)sender;
				int col = this->tableLayoutPanel1->GetColumn(checkBox);
				for(int i=0; i<7; i++)
				{
					int ix = i*24+(col-1);
					this->TimeSlots[ix]->Checked = checkBox->Checked;
				}
		 }
private: System::Void TimeSlotCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				System::Windows::Forms::CheckBox^ checkBox = (CheckBox^)sender;
				int day = this->tableLayoutPanel1->GetRow(checkBox) - 1;
				int hour = this->tableLayoutPanel1->GetColumn(checkBox) - 1;
				
				//DbgPrint("[%d] befor starthour = %d - stopHour = %d", day, m_startHour[day], m_stopHour[day]);
				if(checkBox->Checked)
				{
					if(m_startHour[day]>0 && m_startHour[day]<25 && hour<m_startHour[day])
					{
						for(int i=hour; i<m_startHour[day]; i++)
						{
							this->TimeSlots[i+day*24]->Checked = true;
						}
						m_startHour[day] = hour;
					}
					else if(m_stopHour[day]>0 && m_stopHour[day]<25 && hour>m_stopHour[day])
					{
						for(int i=m_stopHour[day]; i<hour; i++)
						{
							this->TimeSlots[i+day*24]->Checked = true;
						}
						m_stopHour[day] = hour+1;
					}
					else if(hour == m_startHour[day]-1)
						m_startHour[day]--;
					else if(hour == m_stopHour[day])
						m_stopHour[day]++;
					else
					{
						m_startHour[day] = hour;
						m_stopHour[day] = hour+1;
					}
				}
				else
				{
					if(m_startHour[day]<hour && m_stopHour[day]-1>hour)
					{
						for(int i=hour; i<m_stopHour[day]; i++)
						{
							this->TimeSlots[i+day*24]->Checked = false;
						}
						m_stopHour[day] = hour;
					}
					else if(hour == m_startHour[day])
						m_startHour[day] += 1;
					else if(hour == m_stopHour[day]-1)
						m_stopHour[day]--;

					if(m_startHour[day] == m_stopHour[day])
					{
						m_startHour[day] = -1;
						m_stopHour[day] = -1;
					}
				}
				//DbgPrint("[%d] after starthour = %d - stopHour = %d", day, m_startHour[day], m_stopHour[day]);

		 }
private: System::Void enableSchedulingCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 EnableControls(this->enableSchedulingCheckBox->Checked);
		 }
private: System::Void cancelButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void OKButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 Apply();
			 this->Close();
		 }
};
}
