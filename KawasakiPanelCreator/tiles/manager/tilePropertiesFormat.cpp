//---------------------------------------------------------------------------
#include "tileProperties.h"
//---------------------------------------------------------------------------
void TtilePropertiesWindow::prepareColorsView(int elementsVisible[4], bool singleFullLen, bool rightSide)
{
	int buttonWidth = 60;
	int labelButtonOffset = 2;
	//odczytujemy ile jakich jest kolorow
	int backgroundColors = elementsVisible[0];
	int labelColors = elementsVisible[1];
	int lampColors = elementsVisible[2];
	int switchColors = elementsVisible[3];
	//sprawdzamy czy sa jakiekolwiek kolory
	if (backgroundColors+labelColors+lampColors+switchColors > 0) {
		layoutColors->Visible = true;
	} else {
		layoutColors->Visible = false;
	}
	//sprawdzamy czy wyswietlac guziki po prawej stronie
	if (rightSide) {
		layoutColors->Position->X = layoutLabels12->Width;
		layoutColors->Position->Y = layoutLabels12->Position->Y;
	} else {
		if (layoutLabels34->Visible) {
			layoutColors->Position->X = layoutLabels34->Position->X;
			layoutColors->Position->Y = layoutLabels34->Position->Y+layoutLabels34->Height;
		} else {
			if (layoutLabels12->Visible) {
				layoutColors->Position->X = layoutLabels12->Position->X;
				layoutColors->Position->Y = layoutLabels12->Position->Y+layoutLabels12->Height;
			} else {
                layoutColors->Position->X = layoutLabels12->Position->X;
				layoutColors->Position->Y = layoutLabels12->Position->Y;
            }
        }

	}
	/********* KOLOR TLA (BACKGROUND) *********/
	if(backgroundColors > 0 && backgroundColors <= 2 && switchColors == 0) {
		//mamy jakis guzik z kolorami tla - pokazujemy cala grupe
		groupColorBackg->Visible = true;
		groupColorBackg->Position->X = 0;
		groupColorBackg->Position->Y = 0;
		if(backgroundColors == 1) {
			if(labelColors == 0) {
				if(singleFullLen) {
					//zaznaczona flaga ze pojedynczy kolor = pelna szerokosc
					groupColorBackg->Width = layoutColors->Width;
				} else {
					//flaga wylaczona - zajmujemy polowe szerokosci
                    groupColorBackg->Width = layoutColors->Width*0.46;
                }
			} else {
				groupColorBackg->Width = layoutColors->Width*0.5;
			}
			//aktywny jest tylko jeden przycisk = backgroundOff
			titleBackgroundOFF->Visible = false;
			colorButtonBackg_altOff->Visible = true;
			colorButtonBackg_altOff->Width = buttonWidth;
			colorButtonBackg_altOff->Position->X = (groupColorBackg->Width-colorButtonBackg_altOff->Width)/2;
			colorButtonBackg_altOff->Position->Y = 16;
			//drugi przycisk jest schowany
			titleBackgroundON->Visible = false;
			colorButtonBackgOn->Visible = false;
		} else if(backgroundColors == 2) {
			groupColorBackg->Width = layoutColors->Width*0.65;
			//aktywne sa oba przyciski koloru tla
			titleBackgroundON->Visible = true;
			titleBackgroundON->Position->X = 10;
			titleBackgroundON->Position->Y = 25;
			colorButtonBackgOn->Visible = true;
			colorButtonBackgOn->Width = buttonWidth;
			colorButtonBackgOn->Position->X=titleBackgroundON->Position->X+titleBackgroundON->Width+labelButtonOffset;
			colorButtonBackgOn->Position->Y=16;
            //-------------------------------------
			colorButtonBackg_altOff->Visible = true;
			colorButtonBackg_altOff->Width = buttonWidth;
			colorButtonBackg_altOff->Position->X=groupColorBackg->Width-colorButtonBackg_altOff->Width-10;
			colorButtonBackg_altOff->Position->Y=16;
			titleBackgroundOFF->Visible = true;
			titleBackgroundOFF->Position->X = colorButtonBackg_altOff->Position->X-titleBackgroundOFF->Width-labelButtonOffset;
			titleBackgroundOFF->Position->Y = 25;
		}
	} else {
        //brak guzika z kolorami tla - ukrywamy cala grupe
		groupColorBackg->Visible = false;
	}

	/********* KOLOR LAMPKI *********/
	if(lampColors > 0 && lampColors <= 2 && switchColors == 0 && backgroundColors <= 1) {
		//mamy jakis guzik z kolorami tla - pokazujemy cala grupe
		groupColorLamp->Visible = true;
		//sprawdzamy czy sa jakies przyciski tla  bo ma to wplyw na polozenia guzikow lampki
		if(groupColorBackg->Visible) {
			//sa widoczne przyciski tla = lampki beda pokazane pod spodem
			groupColorLamp->Position->X = 0;
			groupColorLamp->Position->Y = groupColorBackg->Height;
			if (singleFullLen) {
				groupColorLamp->Width = layoutColors->Width;
			} else {
				groupColorLamp->Width = layoutColors->Width*0.46;
			}
			groupColorLamp->Width = layoutColors->Width;
		} else {
			//nie ma przyciskow tla = lampki zajmuja ich pozycje !
			groupColorLamp->Position->X = 0;
			groupColorLamp->Position->Y = 0;
			groupColorLamp->Width = layoutColors->Width*0.65;
		}
		//aktywne sa oba przyciski koloru tla
		titleLampON->Visible = true;
		titleLampON->Position->Y = 25;
		colorButtonLampOn->Visible = true;
		colorButtonLampOn->Width = buttonWidth;
		colorButtonLampOn->Position->Y=16;
		//polozenie buttonow z kolorami jest zalezne od szerokosci groupBoxa
		if(groupColorLamp->Width == layoutColors->Width){
			colorButtonLampOn->Position->X = (groupColorLamp->Width-colorButtonLampOn->Width)/2-colorButtonLampOn->Width;
			titleLampON->Position->X = colorButtonLampOn->Position->X-titleLampON->Width-labelButtonOffset;
		} else {
			titleLampON->Position->X = 10;
			colorButtonLampOn->Position->X=titleLampON->Position->X+titleLampON->Width+labelButtonOffset;
		}
		//-------------------------------------
		colorButtonLampOff->Visible = true;
		colorButtonLampOff->Width = buttonWidth;
		colorButtonLampOff->Position->Y=16;
		//polozenie buttonow z kolorami jest zalezne od szerokosci groupBoxa
		if(groupColorLamp->Width == layoutColors->Width){
			colorButtonLampOff->Position->X = (groupColorLamp->Width-colorButtonLampOff->Width)/2+colorButtonLampOff->Width;
			titleLampOFF->Position->X = colorButtonLampOff->Position->X-titleLampOFF->Width-labelButtonOffset;
		} else {
			colorButtonLampOff->Position->X=groupColorLamp->Width-colorButtonLampOff->Width-10;
			titleLampOFF->Position->X = colorButtonLampOff->Position->X-titleLampOFF->Width-labelButtonOffset;
		}
		titleLampOFF->Visible = true;
		titleLampOFF->Position->Y = 25;
	} else {
        //brak guzika z kolorami tla - ukrywamy cala grupe
		groupColorLamp->Visible = false;
	}

	/********* KOLOR PRZELACZNIKA (wystepuje tylko bez kolorow tla i lamp *********/
	if(switchColors > 1 && switchColors <= 3 && lampColors == 0 && backgroundColors == 0) {
		//mamy jakis guzik z kolorami przelacznika - pokazujemy cala grupe
		groupColorSwitch->Visible = true;
		//sa widoczne przyciski tla = lampki beda pokazane pod spodem
		groupColorSwitch->Position->X = 0;
		groupColorSwitch->Position->Y = 0;
		groupColorSwitch->Width = layoutColors->Width;
        //moze byc przelacznik 2 lub 3 polozeniowy (kolorowy)
		if(switchColors == 2) {
			groupColorSwitch->Width = layoutColors->Width*0.65;
			//aktywne sa oba przyciski koloru tla
			titleSwitchL->Visible = true;
			titleSwitchL->Position->X = 10;
			titleSwitchL->Position->Y = 25;
			colorButtonSwitchL->Visible = true;
			colorButtonSwitchL->Width = buttonWidth;
			colorButtonSwitchL->Position->X=titleSwitchL->Position->X+titleSwitchL->Width+labelButtonOffset;
			colorButtonSwitchL->Position->Y=16;
            //-------------------------------------
			colorButtonSwitchR->Visible = true;
			colorButtonSwitchR->Width = buttonWidth;
			colorButtonSwitchR->Position->X=groupColorSwitch->Width-colorButtonSwitchR->Width-10;
			colorButtonSwitchR->Position->Y=16;
			titleSwitchR->Visible = true;
			titleSwitchR->Position->X = colorButtonSwitchR->Position->X-titleSwitchR->Width-labelButtonOffset;
			titleSwitchR->Position->Y = 25;
			//trzeci przycisk jest schowany
			titleSwitchM->Visible = false;
			colorButtonSwitchM->Visible = false;
		} else if(switchColors == 3) {
			groupColorSwitch->Width = layoutColors->Width;
			//aktywne sa wszystkie trzy przyciski koloru przelacznika
			colorButtonSwitchL->Visible = true;
			colorButtonSwitchL->Width = buttonWidth;
			colorButtonSwitchL->Position->X = (groupColorSwitch->Width-buttonWidth)/2-buttonWidth-35;
			colorButtonSwitchL->Position->Y = 16;
			titleSwitchL->Visible = true;
			titleSwitchL->Position->X = colorButtonSwitchL->Position->X-titleSwitchL->Width-labelButtonOffset;
			titleSwitchL->Position->Y = 25;
			//-------------------------------
			colorButtonSwitchR->Visible = true;
			colorButtonSwitchR->Width = buttonWidth;
			colorButtonSwitchR->Position->X = (groupColorSwitch->Width-buttonWidth)/2+buttonWidth+35;
			colorButtonSwitchR->Position->Y = 16;
			titleSwitchR->Visible = true;
			titleSwitchR->Position->X = colorButtonSwitchR->Position->X-titleSwitchR->Width-labelButtonOffset;
			titleSwitchR->Position->Y = 25;
			//-------------------------------
			colorButtonSwitchM->Visible = true;
			colorButtonSwitchM->Width = buttonWidth;
			colorButtonSwitchM->Position->X = (groupColorSwitch->Width-buttonWidth)/2;
			colorButtonSwitchM->Position->Y = 16;
			titleSwitchM->Visible = true;
			titleSwitchM->Position->X = colorButtonSwitchM->Position->X-titleSwitchM->Width-labelButtonOffset;
			titleSwitchM->Position->Y = 25;
		} else {
			//nie moze byc tylko jeden guzik z kolorami przelacznika
			groupColorSwitch->Visible = false;
        }
	} else {
        //brak guzika z kolorami tla - ukrywamy cala grupe
		groupColorSwitch->Visible = false;
	}

	/********* KOLOR LABELA *********/
	if(labelColors > 0 && labelColors <= 1) {
		//mamy guzik z kolorami labela - pokazujemy cala grupe
		groupColorLabel->Visible = true;
		//sprawdzamy ile jest kafelkow z tlem (bo kolory labela sa obok!)
		if(backgroundColors == 1) {
			groupColorLabel->Position->X = groupColorBackg->Width;
			groupColorLabel->Position->Y = 0;
			groupColorLabel->Width = layoutColors->Width*0.5;
		} else if(backgroundColors == 2) {
			groupColorLabel->Position->X = groupColorBackg->Width;
			groupColorLabel->Position->Y = 0;
			groupColorLabel->Width = layoutColors->Width*0.36;
		} else if(lampColors == 2) {
			groupColorLabel->Position->X = groupColorLamp->Width;
			groupColorLabel->Position->Y = 0;
			groupColorLabel->Width = layoutColors->Width*0.36;
		} else if (switchColors == 2) {
			groupColorLabel->Position->X = groupColorSwitch->Width;
			groupColorLabel->Position->Y = 0;
			groupColorLabel->Width = layoutColors->Width*0.36;
		} else if (switchColors == 3) {
			groupColorLabel->Position->X = 0;
			groupColorLabel->Position->Y = groupColorSwitch->Height;
			if (singleFullLen) {
				groupColorLabel->Width = layoutColors->Width;
			} else {
				groupColorLabel->Width = layoutColors->Width*0.46;
			}
		}
		colorButtonLabel->Width = buttonWidth;
		colorButtonLabel->Position->X = (groupColorLabel->Width-colorButtonLabel->Width)/2;
		colorButtonLabel->Position->Y = 16;
	} else {
		//brak labela z kolorami tla - ukrywamy cala grupe
		groupColorLabel->Visible = false;
	}
	//na koniec dobieramy wysokosc calego layoutu z kolorami
	layoutColors->Height = 0;
	for (int i = 0; i < layoutColors->ChildrenCount; i++) {
		if (layoutColors->Children[i]->ClassNameIs("TGroupBox")) {
			TGroupBox *box = dynamic_cast<TGroupBox*>(layoutColors->Children[i]);
			if (box->Visible) {
				//dana grupa jest widoczna - sprawdzamy czy znajduje sie na X = 0
				if (box->Position->X == 0) {
                    layoutColors->Height += box->Height;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::prepareSignalsView(int elementsVisible[4], position layoutPos, position editsPos)
{
	//sprawdzamy czy sa jakiekolwiek kolory
	if (sumInitialValue(elementsVisible,4) > 0) {
		//zmienna informujaca o pokazaniu (lub nie) opcji cyfrowych sygnalow
		int digitalOpt = elementsVisible[3];
		//umieszczamy layout w odpowiednim miejscu w okienku
		placeSignalsLayout(layoutPos);
		//ukladamy w layoucie elementy typu edit (+ ich tytuly)
		placeSignalsEdits(elementsVisible,digitalOpt,editsPos);
		//ukladamy w layoucie elementy dodatkowe dot. sygnalow
		placeSignalsFeatures(digitalOpt);
		//ustawiamy szerokosc i wysokosc tego layoutu i go pokazujemy
		layoutSignals->Width = extremeElementPos(layoutSignals,right);
		layoutSignals->Height = extremeElementPos(layoutSignals,bottom);
		layoutSignals->Visible = true;
	}  else {
		layoutSignals->Visible = false;
	}
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::placeSignalsLayout(position layoutPos)
{
	if (layoutPos==right) {
		//sprawdzamy czy jest widoczny layout z kolorami (przewaznie zawsze jest)
		if(layoutColors->Visible){
			if(layoutColors->Position->X == 0) {
				//layout z kolorami jest po lewej stronie
				layoutSignals->Position->X = layoutLabels12->Width;
				layoutSignals->Position->Y = layoutLabels12->Position->Y;
			} else {
				//layout z kolorami jest po prawej stronie - sygnaly sa zaraz pod nim
				layoutSignals->Position->X = layoutLabels12->Width;
				layoutSignals->Position->Y = layoutColors->Position->Y+layoutColors->Height;
			}
		} else {
			//layout z kolorami niewidoczny - mozemy smialo pokazywac sygnaly na gorze po prawo
			layoutSignals->Position->X = layoutLabels12->Width;
			layoutSignals->Position->Y = layoutLabels12->Position->Y;
		}
	} else if (layoutPos==left) {
		//sprawdzamy czy jest widoczny layout z kolorami (przewaznie zawsze jest)
		if(layoutColors->Visible){
			if(layoutColors->Position->X == 0) {
				//layout z kolorami jest po lewej stronie - sygnaly sa zaraz pod nim
				layoutSignals->Position->X = 0;
				layoutSignals->Position->Y = layoutColors->Position->Y+layoutColors->Height;
			} else {
				//layout z kolorami jest po prawej stronie
				layoutSignals->Position->X = layoutLabels34->Position->X;
				layoutSignals->Position->Y = layoutLabels34->Position->Y+layoutLabels34->Height;
			}
		} else {
			layoutSignals->Position->X = layoutLabels34->Position->X;
			layoutSignals->Position->Y = layoutLabels34->Position->Y+layoutLabels34->Height;
		}
	} else {
        //nie obslugiwany tryb
    }
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::placeSignalsEdits(int elementsVisible[3],int digitalOpt, position editsPos)
{
	//odczytujemy ile jakich jest sygnalow
	int editWidth = 130, offstetH = 18, offsetV = 5;
	int backgroundSignals = elementsVisible[0];
	int lampSignals = elementsVisible[1];
	int switchSignals = elementsVisible[2];
		//sprawdzamy ile jest sygnalow dla guzika
		if(backgroundSignals==1) {
			int positionInitialX = 5, positionInitialY = 5;
			//mamy pole do sygnalu guzika - ustawiamy odpowiednio label
			titleButtonSignal_altTop->Position->X = positionInitialX;
			titleButtonSignal_altTop->Position->Y = positionInitialY;
			if(digitalOpt>0) {
				titleButtonSignal_altTop->Text = "TOP SIGNAL NO.";
			} else {
				titleButtonSignal_altTop->Text = "BUTTON SIGNAL";
			}
			titleButtonSignal_altTop->Visible = true;
			//teray ustawiamy samo pole edycyjne
			editButtonSignal_altTop->Width = editWidth;
			editButtonSignal_altTop->Position->X = titleButtonSignal_altTop->Position->X+2;
			editButtonSignal_altTop->Position->Y = titleButtonSignal_altTop->Position->Y+titleButtonSignal_altTop->Height-3;
			editButtonSignal_altTop->Visible = true;
		} else {
			titleButtonSignal_altTop->Visible = false;
			editButtonSignal_altTop->Visible = false;
		}
		//-----------------------------------------
		//sprawdzamy ile jest sygnalow dla lampki
		if(lampSignals==1) {
			int positionInitialX, positionInitialY;
        	//jezeli byl sygnal do guzika to sygnal lampki jest ponize
			if(backgroundSignals==1) {
				if (editsPos==vertical) {
					positionInitialX = titleButtonSignal_altTop->Position->X;
					positionInitialY = editButtonSignal_altTop->Position->Y+editButtonSignal_altTop->Height+offsetV;
				} else if (editsPos==horizontal) {
					positionInitialX = titleButtonSignal_altTop->Position->X+editButtonSignal_altTop->Width+offstetH;
					positionInitialY = titleButtonSignal_altTop->Position->Y;
				} else {
				   //not such case
				}
			} else {
				positionInitialX = 5;
				positionInitialY = 5;
			}
			titleLampSignal->Position->X = positionInitialX;
			titleLampSignal->Position->Y = positionInitialY;
			titleLampSignal->Visible = true;
			//teraz ustawiamy samo pole edycyjne
			editLampSignal->Width = editWidth;
			editLampSignal->Position->X = titleLampSignal->Position->X+2;
			editLampSignal->Position->Y = titleLampSignal->Position->Y+titleLampSignal->Height-3;
			editLampSignal->Visible = true;
		} else {
			titleLampSignal->Visible = false;
			editLampSignal->Visible = false;
		}
		//-----------------------------------------
		//sprawdzamy ile jest sygnalow dla przelacznika
		if(switchSignals==2 || switchSignals==3) {
			int positionInitialX, positionInitialY;
			//sprawdzamy czy nie ma sygnalu guzika ORAZ sygnalu lampki
			if(backgroundSignals==0 && lampSignals==0) {
				positionInitialX = 5;
				positionInitialY = 5;
			} else {
				//jeden z tych elementow ma byc widoczne na scenie !
				if(lampSignals==1) {
					//widoczny jest signal lamp (od niego sie odnosimy)
					positionInitialX = titleLampSignal->Position->X;
					positionInitialY = editLampSignal->Position->Y+editLampSignal->Height+offsetV;
				} else {
					//widoczny jest button lamp (od niego sie odnosimy
					positionInitialX = titleButtonSignal_altTop->Position->X;
					positionInitialY = editButtonSignal_altTop->Position->Y+editButtonSignal_altTop->Height+offsetV;
                }
			}
			titleSwitchSignalL_altUp->Position->X = positionInitialX;
			titleSwitchSignalL_altUp->Position->Y = positionInitialY;
			if (dispTypeRotation->IsChecked) {
            	titleSwitchSignalL_altUp->Text = "SWITCH L";
			} else if(dispTypeUpDown->IsChecked) {
				titleSwitchSignalL_altUp->Text = "SWITCH U";
            }
			titleSwitchSignalL_altUp->Visible = true;
			//teraz ustawiamy samo pole edycyjne
			editSwitchSignalL_altUp->Width = editWidth;
			editSwitchSignalL_altUp->Position->X = titleSwitchSignalL_altUp->Position->X+2;
			editSwitchSignalL_altUp->Position->Y = titleSwitchSignalL_altUp->Position->Y+titleSwitchSignalL_altUp->Height-3;
			editSwitchSignalL_altUp->Visible = true;
			//jezeli maja byc 3 guziki to teraz bedzie srodkowy
			if (switchSignals==3) {
				//sprawdzamy czy guziki maja byc pionowo czy poziomo
				if (editsPos==vertical) {
					//ustawiamy szerokosc poprzedniego guzika (switchL)
					editSwitchSignalL_altUp->Width = editWidth;
					//ustalamy poyzcje nowego detalu
					titleSwitchSignalM->Position->X = editSwitchSignalL_altUp->Position->X;
					titleSwitchSignalM->Position->Y = editSwitchSignalL_altUp->Position->Y+editSwitchSignalL_altUp->Height+offsetV;
					titleSwitchSignalM->Visible = true;
					//teraz ustawiamy samo pole edycyjne
					editSwitchSignalM->Width = editWidth;
					editSwitchSignalM->Position->X = titleSwitchSignalM->Position->X+2;
					editSwitchSignalM->Position->Y = titleSwitchSignalM->Position->Y+titleSwitchSignalM->Height-3;
					editSwitchSignalM->Width = editWidth;
					editSwitchSignalM->Visible = true;
				} else if (editsPos==horizontal) {
					//ustawiamy szerokosc poprzedniego guzika (switchL)
					editSwitchSignalL_altUp->Width = 0.65*editWidth;
                    //ustalamy poyzcje nowego detalu
					titleSwitchSignalM->Position->X = titleSwitchSignalL_altUp->Position->X+editSwitchSignalL_altUp->Width+offstetH;
					titleSwitchSignalM->Position->Y = titleSwitchSignalL_altUp->Position->Y;
					titleSwitchSignalM->Visible = true;
					//teraz ustawiamy samo pole edycyjne
					editSwitchSignalM->Width = editWidth;
					editSwitchSignalM->Position->X = titleSwitchSignalM->Position->X+2;
					editSwitchSignalM->Position->Y = titleSwitchSignalM->Position->Y+titleSwitchSignalM->Height-3;
					editSwitchSignalM->Width = 0.65*editWidth;
					editSwitchSignalM->Visible = true;
				} else {
					//not supported
				}
			} else {
				titleSwitchSignalM->Visible = false;
				editSwitchSignalM->Visible = false;
            }
			//na koncu dajemy guzik prawy - sprawdzamy czy byl guzik srodkowy
			if(titleSwitchSignalM->Visible) {
				//jest edit switchM - sprawdzamy czy od niego mamy przesuac w pionie czy poziomie
				if (editsPos==vertical) {
					//przesuwamy sie w pionie
					positionInitialX = titleSwitchSignalM->Position->X;
					positionInitialY = editSwitchSignalM->Position->Y+editSwitchSignalM->Height+offsetV;
					editSwitchSignalR_altDown->Width = editWidth;
				} else if (editsPos==horizontal) {
					//przesuwamy sie w poziomie
					positionInitialX = titleSwitchSignalM->Position->X+editSwitchSignalM->Width+offstetH;
					positionInitialY = titleSwitchSignalM->Position->Y;
					editSwitchSignalR_altDown->Width = 0.65*editWidth;
                }
			} else {
				//nie ma edita switchM - sprawdzamy czy przesuac sie w pionie czy poziomie od switchL
				if (editsPos==vertical) {
					//przesuwamy sie w pionie
					positionInitialX = titleSwitchSignalL_altUp->Position->X;
					positionInitialY = editSwitchSignalL_altUp->Position->Y+editSwitchSignalL_altUp->Height+offsetV;
					editSwitchSignalR_altDown->Width = editWidth;
				} else if (editsPos==horizontal) {
					//przesuwamy sie w poziomie
					positionInitialX = titleSwitchSignalL_altUp->Position->X+editSwitchSignalL_altUp->Width+offstetH;
					positionInitialY = titleSwitchSignalL_altUp->Position->Y;
					editSwitchSignalR_altDown->Width = editWidth;
                }
            }
			titleSwitchSignalR_altDown->Position->X = positionInitialX;
			titleSwitchSignalR_altDown->Position->Y = positionInitialY;
			if (dispTypeRotation->IsChecked) {
				titleSwitchSignalR_altDown->Text = "SWITCH R";
			} else if(dispTypeUpDown->IsChecked) {
				titleSwitchSignalR_altDown->Text = "SWITCH D";
            }
			titleSwitchSignalR_altDown->Visible = true;
			//teraz ustawiamy samo pole edycyjne
			editSwitchSignalR_altDown->Position->X = titleSwitchSignalR_altDown->Position->X+2;
			editSwitchSignalR_altDown->Position->Y = titleSwitchSignalR_altDown->Position->Y+titleSwitchSignalR_altDown->Height-3;
			editSwitchSignalR_altDown->Visible = true;
		}  else {
			//zla liczba - wszystko ZNIKA !!!
			titleSwitchSignalL_altUp->Visible = false;
			editSwitchSignalL_altUp->Visible = false;
			titleSwitchSignalM->Visible = false;
			editSwitchSignalM->Visible = false;
			titleSwitchSignalR_altDown->Visible = false;
			editSwitchSignalR_altDown->Visible = false;
		}
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::placeSignalsFeatures(int digitalOpt)
{
	//sprawdzamy jakie opcje chcemy pokazac
	if(digitalOpt>0 && digitalOpt<=3) {
		//pokaz tylko liczbe znakow (figures no.) + jego tytul
		spinBoxFigures->Visible = true;
		titleSpinFigures->Visible = true;
		//jezeli aktywna jest 2 to pokazujemy kolejne itemy / jak nie to chowamy
		if (digitalOpt>1) {
			//ustawiamy spin z liczba znakow + jego tytul
			spinBoxFigures->Position->X = 145;
			spinBoxFigures->Position->Y = 17;
			spinBoxFigures->Width = 70;
			titleSpinFigures->Position->X = spinBoxFigures->Position->X+(spinBoxFigures->Width-titleSpinFigures->Width)/2;
			titleSpinFigures->Position->Y = spinBoxFigures->Position->Y-12;
			//pokazujemy drugiego spinBoxa i jego tytul
			spinBoxSigNo->Visible = true;
			spinBoxSigNo->Position->X = spinBoxFigures->Position->X + spinBoxFigures->Width + 3;
			spinBoxSigNo->Position->Y = spinBoxFigures->Position->Y;
			spinBoxSigNo->Width = 70;
			titleSpinSigNo->Visible = true;
			titleSpinSigNo->Position->X = spinBoxSigNo->Position->X+(spinBoxSigNo->Width-titleSpinSigNo->Width)/2;
			titleSpinSigNo->Position->Y = spinBoxFigures->Position->Y-12;
			//pokazujemy groupBox SIGNAL TYPE
			groupSignalType->Visible = true;
			groupSignalType->Position->X = spinBoxFigures->Position->X - 2;
			groupSignalType->Position->Y = spinBoxSigNo->Position->Y + spinBoxSigNo->Height + 5;
			//pokazujemy checkBox MINUS VALUE
			checkBoxMinusVal->Visible = true;
			checkBoxMinusVal->Position->X = 15;
			checkBoxMinusVal->Position->Y = 60;
			//jezeli aktywna jest 3 to pokazujemy wszystko do konca  / jak nie to chowamy
			if (digitalOpt>2) {
				//pokazujemy ostatnie dwie grupy ustawien i je ustawiamy
				groupDataLimit->Visible = true;
				groupDataLimit->Position->X = 3;
				groupDataLimit->Position->Y = 55;
				groupInputType->Visible = true;
				groupInputType->Position->X = groupSignalType->Position->X;
				groupInputType->Position->Y = groupSignalType->Position->Y + groupSignalType->Height + 5;
				//zmieniamy pozycje checkBoxa z minus val
				checkBoxMinusVal->Position->X = groupInputType->Position->X+(groupInputType->Width-checkBoxMinusVal->Width)/2;
				checkBoxMinusVal->Position->Y = groupInputType->Position->Y+groupInputType->Height+1;
			} else {
				groupDataLimit->Visible = false;
				groupInputType->Visible = false;
            }
		} else {
			//centrujemy jedynego spinBoxa i jego tytul
			spinBoxFigures->Position->X = 7;
			spinBoxFigures->Position->Y = 15;
			spinBoxFigures->Width = 100;
			titleSpinFigures->Position->X = spinBoxFigures->Position->X+(spinBoxFigures->Width-titleSpinFigures->Width)/2;
			titleSpinFigures->Position->Y = spinBoxFigures->Position->Y-12;
			//------chowamy wszystko inne
			spinBoxSigNo->Visible = false;
			titleSpinSigNo->Visible = false;
			groupSignalType->Visible = false;
			checkBoxMinusVal->Visible = false;
			groupDataLimit->Visible = false;
			groupInputType->Visible = false;
        }
		layoutSignalFeatures->Visible = true;
		//ustawiamy wysokosc i szerokosc tego layoutu
		layoutSignalFeatures->Width = extremeElementPos(layoutSignalFeatures,right);
		layoutSignalFeatures->Height = extremeElementPos(layoutSignalFeatures,bottom);
	} else {
		//wszystkie opcje schowaj
		layoutSignalFeatures->Visible = false;
	}
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::prepareVariablesView(int elementsVisible[2],bool rightSide)
{
	bool varEditVisible = elementsVisible[0];
	bool varTypeVisible = elementsVisible[1];
	int positionY = layoutLabels12->Position->Y;

	if (sumInitialValue(elementsVisible,2)>0) {
		//pokazujemy elementy
		if (varEditVisible) {
			titleVariableName->Visible = true;
			editVariableName->Visible = true;
			if (!varTypeVisible) {
            	editVariableName->Width = 275;
			} else {
				editVariableName->Width = 145;
            }
		}
		groupDataType->Visible = varTypeVisible;
		//sprawdzamy czy pokazac po prawej stronie
		if (rightSide) {
			//czy sa widoczne kolory i tez sa po prawej stronie
			if(layoutColors->Visible == true && layoutColors->Position->X == layoutLabels12->Width) {
                positionY += layoutColors->Height;
			}
			//czy sa widoczne sygnaly i tez sa po prawej stronie
			if(layoutSignals->Visible == true && layoutSignals->Position->X == layoutLabels12->Width) {
				positionY += layoutSignals->Height;
			}
			layoutVariables->Position->X = layoutLabels12->Width;
			layoutVariables->Position->Y = positionY;
		} else {
			positionY += layoutLabels12->Height;
			//czy sa widoczne dwa ostatnie labele i tez sa po lewej stronie
			if(layoutLabels34->Visible == true && layoutLabels34->Position->X == 0) {
				positionY += layoutLabels34->Height;
			}
			//czy sa widoczne kolory i tez sa po lewej stronie
			if(layoutColors->Visible == true && layoutColors->Position->X == 0) {
				positionY += layoutColors->Height;
			}
			//czy sa widoczne sygnaly i tez sa po prawej stronie
			if(layoutSignals->Visible == true && layoutSignals->Position->X == 0) {
				positionY += layoutSignals->Height;
			}
			layoutVariables->Position->X = 0;
			layoutVariables->Position->Y = positionY;
		}
		//pokazujemy layout
		layoutVariables->Visible = true;
	} else {
         layoutVariables->Visible = false;
    }
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::prepareMiscView(componentPos elements[3],bool rightSide)
{
	//pierwszy element to checkbox OPERATION
	checkBoxOperationCond->Visible = elements[0].visible;
	checkBoxOperationCond->Position->X = elements[0].x;
	checkBoxOperationCond->Position->Y = elements[0].y;
	//kolejny element to checkbox EXCLAMATION
	checkBoxExclMark->Visible = elements[1].visible;
	checkBoxExclMark->Position->X = elements[1].x;
	checkBoxExclMark->Position->Y = elements[1].y;
	//kolejny element to groupBox DISP TYPE
	groupDispType->Visible = elements[2].visible;
	groupDispType->Position->X = elements[2].x;
	groupDispType->Position->Y = elements[2].y;
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::prepareSpecialView(specialView viewNo,bool rightSide)
{
	if (viewNo!=none) {
		TLayout *currLayout;
		int positionY = layoutLabels12->Position->Y;
		if (viewNo==palletize) {
			currLayout = layoutPalletize;
			layoutServo->Visible = false;
			layoutCmdString->Visible = false;
			layoutStrWindow->Visible = false;
		} else if (viewNo==monitorCmd) {
			currLayout = layoutCmdString;
			layoutServo->Visible = false;
			layoutPalletize->Visible = false;
			layoutStrWindow->Visible = false;
		} else if (viewNo==stringWindow) {
			currLayout = layoutStrWindow;
			layoutServo->Visible = false;
			layoutPalletize->Visible = false;
			layoutCmdString->Visible = false;
		} else if (viewNo==servo) {
			currLayout = layoutServo;
			layoutPalletize->Visible = false;
			layoutStrWindow->Visible = false;
			layoutCmdString->Visible = false;
		}
		//sprawdzamy po ktorej stronie pokazac wybrany layout
		if (rightSide) {
			//czy sa widoczne kolory i tez sa po prawej stronie
			if(layoutColors->Visible == true && layoutColors->Position->X == layoutLabels12->Width) {
                positionY += layoutColors->Height;
			}
			//czy sa widoczne sygnaly i tez sa po prawej stronie
			if(layoutSignals->Visible == true && layoutSignals->Position->X == layoutLabels12->Width) {
				positionY += layoutCmdString->Height;
			}
			currLayout->Position->X = layoutLabels12->Width;
			currLayout->Position->Y = positionY;
		} else {
			//czy sa widoczne dwa ostatnie labele i tez sa po lewej stronie
			if(layoutLabels12->Visible == true && layoutLabels12->Position->X == 0) {
				positionY += layoutLabels12->Height;
			}
			//czy sa widoczne dwa ostatnie labele i tez sa po lewej stronie
			if(layoutLabels34->Visible == true && layoutLabels34->Position->X == 0) {
				positionY += layoutLabels34->Height;
			}
			//czy sa widoczne kolory i tez sa po lewej stronie
			if(layoutColors->Visible == true && layoutColors->Position->X == 0) {
				positionY += layoutColors->Height;
			}
			//czy sa widoczne sygnaly i tez sa po prawej stronie
			if(layoutSignals->Visible == true && layoutSignals->Position->X == 0) {
				positionY += layoutCmdString->Height;
			}
			currLayout->Position->X = 0;
			currLayout->Position->Y = positionY;
		}
		//wykonujemy akcje na wybranym layoucie
		currLayout->Visible = true;
	} else {
		layoutServo->Visible = false;
		layoutPalletize->Visible = false;
		layoutCmdString->Visible = false;
		layoutStrWindow->Visible = false;
    }
}
//---------------------------------------------------------------------------
bool TtilePropertiesWindow::entryExistsInTable(String entry, String table[],int size)
{
	bool result = false;

	for (int i = 0; i < size; i++) {
		if(table[i] == entry){
			result = true;
			break;
		}
	}
	return result;
}
//---------------------------------------------------------------------------
int TtilePropertiesWindow::sumInitialValue(int table[],int size)
{
	int sum = 0;

	for (int i = 0; i < size; i++) {
		sum += table[i];
	}

	return sum;
}
//---------------------------------------------------------------------------
int TtilePropertiesWindow::extremeElementPos(TLayout *layout,position poz)
{
	int result = 0, currVal, mostExtremeVal;

	//ustawiamy poczatkowa wartosc najbardziej oddalonego elementu
	if (poz==bottom || poz==right) {
        mostExtremeVal = -1E5;
	} else if (poz==up || poz==left) {
        mostExtremeVal = 1E5;
	}
    //jedziemy po wszystkich elementach w layoucie !
	for (int i = 0; i < layout->ChildrenCount; i++) {
		TControl *comp = dynamic_cast<TControl*>(layout->Children[i]);
		//sprawdzamy czy dany komponent istnieje i jest widoczny
		if (comp && comp->Visible) {
			//dany komponent jest widoczny - pobieramy skrajna pozycje zajmowana
			if (poz==bottom) {
				currVal = comp->Position->Y + comp->Height;
				if (currVal>mostExtremeVal) mostExtremeVal = currVal;
			} else if (poz == up) {
				currVal = comp->Position->Y;
				if (currVal<mostExtremeVal) mostExtremeVal = currVal;
			} else if (poz == right) {
				currVal = comp->Position->X + comp->Width;
				if (currVal>mostExtremeVal) mostExtremeVal = currVal;
			} else if (poz == left) {
				currVal = comp->Position->X;
				if (currVal<mostExtremeVal) mostExtremeVal = currVal;
			} else {
				//jakis blad rzuc wyjatkiem !!!
				throw -1E10;
			}
		}
	}
	result = mostExtremeVal;

	return result;
}
//---------------------------------------------------------------------------

