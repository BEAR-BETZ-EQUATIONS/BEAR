/* 
 * File:   bear_gui_root.h
 * Author: winckler
 *
 * Created on August 10, 2015, 6:36 PM
 */

#ifndef BEAR_GUI_ROOT_H
#define	BEAR_GUI_ROOT_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <iostream>     
#include <sstream>      // std::stringstream

#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"

#include "logger.h"
#include "def.h"

namespace bear
{
    class bear_gui_root
    {
        
    public:
        
        enum method {kDiagonalization,kRungeKutta};
        
        bear_gui_root() :   fCanvas(nullptr), 
                            fLegend(nullptr),   
                            fFunctions(),   
                            fXmin(0.),  
                            fXmax(20.),     
                            fYmin(0.),  
                            fYmax(1.1),
                            fNpoint(1000)
        {
        }
        virtual ~bear_gui_root()
        {
            for(auto& p : fFunctions)
                if(p.second)
                    delete p.second;
            
            if(fLegend)
                delete fLegend;
            
            if(fCanvas)
                delete fCanvas;
            
            
        }
        
        int init(const variables_map& vm)
        {
            
            fXmin=vm.at("thickness.minimum").template as<double>();
            fXmax=vm.at("thickness.maximum").template as<double>();
            fNpoint=vm.at("thickness.point.number").template as<std::size_t>();
            
            fYmin=vm.at("fraction.minimum").template as<double>();
            fYmax=vm.at("fraction.maximum").template as<double>();
            
            std::string proj_symbol=vm.at("projectile.symbol").template as<std::string>();
            std::string proj_energy=vm.at("projectile.energy").template as<std::string>();
            std::string target_symbol=vm.at("target.symbol").template as<std::string>();
            int tmass=(int)vm.at("target.mass.number").template as<double>();
            std::string target_mass=std::to_string(tmass);
            std::string target_pressure=vm.at("target.pressure").template as<std::string>();
            std::string X_unit=vm.at("thickness.unit").template as<std::string>();
            
            std::stringstream ss;
            
            ss<<proj_symbol
                    <<" projectile at "<<proj_energy
                    <<" on ^{"
                    <<target_mass<<"}"
                    <<target_symbol
                    <<" target"
                    ;
            ss<<" with "<< target_pressure <<" pressure.";
            
            fTitle=ss.str();
            fXTitle="thickness (";
            if(X_unit!="mug/cm2")
                fXTitle+=X_unit;
            else
                fXTitle+="#mug/cm2";
            fXTitle+=")";
            
            
            fYTitle="Fractions";
            
            fLegend = new TLegend(0.4,0.7,0.9,0.9);
            fLegend->SetNColumns(4);
            //fXmin=vm.at("thickness.minimum").template as<double>();
            return 0;
        }
        
        
        
        
        // 
        int plot(const std::map<std::size_t, std::string>& input_functions)
        {
            fMethod=kDiagonalization;
            fTitle+=" (Diagonalization method)";
            
            
            for(const auto& p : input_functions)
            {
                std::string tf1_name("f");
                std::string name = "F" + std::to_string(p.first+1);
                tf1_name+=name;
                fFunctions[p.first] = new TF1(name.c_str(), p.second.c_str(), fXmin, fXmax);
                fFunctions.at(p.first)->SetNpx(fNpoint);
                fFunctions.at(p.first)->SetLineColor(p.first+1);
                
                fLegend->AddEntry(fFunctions[p.first], name.c_str());
                
                
            }
            draw(fFunctions);
            return 0;
        }
        
        
        int plot(std::map<std::size_t, std::shared_ptr<TH1D> >& input_functions)
        {
            fMethod=kRungeKutta;
            fTitle+=" (Runge Kutta method)";
            for(auto& p : input_functions)
            {
                std::string name = "F" + std::to_string(p.first+1);
                p.second->SetLineColor(p.first+1);
                p.second->SetLineWidth(2);
                p.second->SetStats(kFALSE);
                fLegend->AddEntry(p.second.get(), name.c_str());
            }
            draw(input_functions);
            return 0;
        }
        
        template <typename T>
        int draw(std::map<std::size_t,T>& container_map)
        {
            LOG(DEBUG)<<"GUI start";
            
            if(fMethod==kDiagonalization)
                fCanvas = new TCanvas("c1Dia","Solutions - Diagonalization",800,600);
            
            
            if(fMethod==kRungeKutta)
                fCanvas = new TCanvas("c1RK","Solutions - Runge Kutta",800,600);
            
            
            if(fMethod!=kRungeKutta && fMethod!=kDiagonalization)
                throw std::runtime_error("Unrecognized method to solve the equations");
            
            //fCanvas->SetLogx(0);
            for(const auto& p : container_map)
            {
                
                if(p.first!=0)
                    p.second->Draw("SAME");
                else
                {
                    //p.second->GetXaxis()->CenterTitle(true);
                    //p.second->GetYaxis()->CenterTitle(true);
                    p.second->GetYaxis()->SetRangeUser(fYmin,fYmax);
                    
                    p.second->GetXaxis()->SetTitle(fXTitle.c_str());
                    p.second->GetYaxis()->SetTitle(fYTitle.c_str());
                    
                    
                    
                    
                    p.second->SetTitle(fTitle.c_str());
                    p.second->Draw();
                }
            }
            
            fCanvas->SetLogx();
            fLegend->Draw();
            return 0;
        }
        
    private:
        TCanvas* fCanvas;
        TLegend* fLegend;
        //TF1 *fa1;
        std::map<std::size_t, std::string> fInput;
        std::map<std::size_t, TF1*> fFunctions;
        double fXmin;
        double fXmax;
        double fYmin;
        double fYmax;
        std::string fTitle;
        std::string fXTitle;
        std::string fYTitle;
        std::size_t fNpoint;
        enum method fMethod;
    };
}


#endif	/* BEAR_GUI_ROOT_H */
