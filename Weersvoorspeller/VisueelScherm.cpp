/*
 * VisueelScherm.cpp
 *
 *  Created on: 11 mrt. 2012
 *      Author: acer
 */

#include "VisueelScherm.hpp"

VisueelScherm::VisueelScherm( WeerData* weerData )
{
	//sla de weerdata op in het attribuut
	this->weerData = weerData;

	//ken font en skin toe
	this->font = new Font(RES_FONT);
	this->skin = new WidgetSkin(RES_SELECTED, RES_UNSELECTED, 16,32,16,32,false, false);

	//maak een achtergrond label om alle andere widgets in op te slaan, en te tonen
	Label* achtergrond = new Label(0,0,0,0,NULL);
	achtergrond->setBackgroundColor(0x000000);

	//maak een listbox waar update en visueelknop aan toegevoegd worden
	this->listBox = new ListBox(5,0,150,40,achtergrond);

	//knop om data te updaten
	this->updateKnop = new Label(5,0,50,25,achtergrond, "Update!", 0, font);
	this->updateKnop->setSkin( this->skin );

	//knop om naar visueel scherm te schakelen
	this->textueelKnop = new Label(55,0,50,25,achtergrond, "Visueel!", 0, font);
	this->textueelKnop->setSkin( this->skin );


	//staafdiagram

	//maak eerst een placeholder
	this->diagramTekening = maCreatePlaceholder();

	//laat de placeholder tekenbaar zijn
	maCreateDrawableImage( this->diagramTekening, EXTENT_X( maGetScrSize() ), EXTENT_Y( maGetScrSize() ) - 30 );

	//mak een nieuwe image met de placeholder
	this->diagramImage = new Image( 0, 30, EXTENT_X( maGetScrSize() ), EXTENT_Y( maGetScrSize() ) - 30, achtergrond, true, true, this->diagramTekening );


	this->setMain( achtergrond );
	this->update();
}


VisueelScherm::~VisueelScherm()
{
}


void VisueelScherm::update()
{
	//update waarden vam weerdata
	this->weerData->update();

	//stel draw target in op onze tekening
	maSetDrawTarget( this->diagramTekening );

	//teken een staaf diagram

	//legenda
	//teken blokje en text met zonneschijn, neerslag en temperatuur
	maSetColor(0xffffff);
	maDrawText(10, 10, "Legenda");
	maDrawText(10, 25, "Zonneschijn");
	maDrawText(10, 40, "Neerslag");
	maDrawText(10, 55, "Temperatuur");

	//zonneschijn
	maSetColor(0xffff00);
	maFillRect(100, 25, 15, 15);

	//neerslag
	maSetColor(0x0000ff);
	maFillRect(100, 40, 15, 15);

	//temperatuur
	maSetColor(0xff0000);
	maFillRect(100, 55, 15, 15);

	//teken 10-stap lijnen
	maSetColor(0xffffff);

	//grid-lijnen
	for(int i=100; i<=200; i+=10)
	{
		maLine(20, i, 200, i);
	}

	//teken de staven van zonneschijn, neerslag en minimum temperatuur

	// Zonneschijn
	maSetColor(0xffff00);
	maFillRect(30, 100+(100-this->weerData->zonneschijn[0]), 10, this->weerData->zonneschijn[0]); //vandaag
	maFillRect(45, 100+(100-this->weerData->zonneschijn[1]), 10, this->weerData->zonneschijn[1]); //morgen
	maFillRect(60, 100+(100-this->weerData->zonneschijn[2]), 10, this->weerData->zonneschijn[2]); //overmorgen

	// Neerslag
	maSetColor(0x0000ff);
	maFillRect(85, 100+(100-this->weerData->neerslag[0]), 10, this->weerData->neerslag[0]); //vandaag
	maFillRect(100, 100+(100-this->weerData->neerslag[1]), 10, this->weerData->neerslag[1]); //morgen
	maFillRect(115, 100+(100-this->weerData->neerslag[2]), 10, this->weerData->neerslag[2]); //overmorgen

	// Temperatuur
	maSetColor(0xff0000);
	maFillRect(140, 100+(100-this->weerData->minimumtemperatuur[0]), 10, this->weerData->minimumtemperatuur[0]); //vandaag
	maFillRect(155, 100+(100-this->weerData->minimumtemperatuur[1]), 10, this->weerData->minimumtemperatuur[1]); //morgen
	maFillRect(170, 100+(100-this->weerData->minimumtemperatuur[2]), 10, this->weerData->minimumtemperatuur[2]); //overmorgen

	//altijd draw target na tekenen teruggeven naar scherm!
	maSetDrawTarget( HANDLE_SCREEN );

	//update de image met de nieuwe tekening
	this->diagramImage->setResource( this->diagramTekening );
}



void VisueelScherm::setToggleScherm( Screen* toggleScherm )
{
	//switch naar andere scherm (textueel scherm)
	this->toggleScherm = toggleScherm;
}




void VisueelScherm::pointerPressEvent( MAPoint2d maPoint )
{
	Point point;
	point.x = maPoint.x;
	point.y = maPoint.y;

	//update knop is ingedrukt
	if( this->updateKnop->contains( point ) )
	{
		this->updateKnop->setSelected( true );
		this->update();
	}
	else
	{
		this->updateKnop->setSelected( false );
	}


	//textueel knop is ingedrukt
	if( this->textueelKnop->contains( point ) )
	{
		this->textueelKnop->setSelected( true );
		this->hide();
		this->toggleScherm->show();
	}
	else
	{
		this->textueelKnop->setSelected( false );
	}
}


