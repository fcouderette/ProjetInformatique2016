#include"toolply.h"
#include"fstream"

std::vector<float> vectX;
std::vector<float> vectY;
std::vector<float> vectZ;
std::vector<int> vectR;
std::vector<int> vectG;
std::vector<int> vectB;
std::vector<int> vectA;

std::vector<float> vectXin;
std::vector<float> vectYin;
std::vector<float> vectZin;
std::vector<int> vectRin;
std::vector<int> vectGin;
std::vector<int> vectBin;
std::vector<int> vectAin;

std::vector<float> vectXout;
std::vector<float> vectYout;
std::vector<float> vectZout;
std::vector<int> vectRout;
std::vector<int> vectGout;
std::vector<int> vectBout;
std::vector<int> vectAout;

/*
float mhuemin1=240;
float mhuemin2=240;
float msatmin=52;
float mlightmin=40;

float mhuemax1=240;
float mhuemax2=240;
float msatmax=80;
float mlightmax=90;
*/


// Callbacks for ply reading
static int x_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    vectX.push_back(ply_get_argument_value(argument));
    //std::cout<<"Yo ! "<<ply_get_argument_value(argument)<<std::endl;
    return 1;
}


static int y_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    vectY.push_back(ply_get_argument_value(argument));
    return 1;
}


static int z_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    vectZ.push_back(ply_get_argument_value(argument));
    return 1;
}


static int r_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    vectR.push_back(ply_get_argument_value(argument));
    return 1;
}


static int g_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    vectG.push_back(ply_get_argument_value(argument));
    return 1;
}


static int b_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    vectB.push_back(ply_get_argument_value(argument));
    return 1;
}


static int alpha_cb(p_ply_argument argument)
{
    long length, value_index;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    vectA.push_back(ply_get_argument_value(argument));
    return 1;
}

// Callback for creation of ply
void error_cb(p_ply ply, const char *message)
{
    //ply_get_argument_user_data(argument, NULL, &eol);
    printf("\nYou made an Error\n");
}






/*
static int setup_callbacks(p_ply inputply, p_ply outputply1, p_ply outputply2)
{
    p_ply_element element = NULL;

        while ((element = ply_get_next_element(iply, element))) {
            p_ply_property property = NULL;
            long ninstances = 0;
            const char *element_name;
            ply_get_element_info(element, &element_name, &ninstances);

            if (!ply_add_element(oply, element_name, ninstances)) return 0;

            while ((property = ply_get_next_property(element, property))) {
                const char *property_name;
                e_ply_type type, length_type, value_type;
                ply_get_property_info(property, &property_name, &type,
                        &length_type, &value_type);

                if (!ply_set_read_cb(iply, element_name, property_name, callback,
                        oply, 0)) return 0;

                if (!ply_add_property(oply, property_name, type, length_type,
                        value_type)) return 0;
            }
        }
        return 1;
}
*/


void readply(MainWindow* win,const char* name1, const char* name2, const char* name3)
{
    p_ply myply=ply_open(name1,NULL, 0, NULL);
    std::cout<<"nom ply lu : "<<name1<<std::endl; //.toStdString()

    // Reads header
    int reshead=ply_read_header(myply);
    std::cout<<"reshead="<<reshead<<std::endl;

    // Returns number of vertices
    //long nbVertices=ply_set_read_cb(myply, "vertex", "x", vertex_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "x", x_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "y", y_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "z", z_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "red", r_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "green", g_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "blue", b_cb, NULL, 0);
    ply_set_read_cb(myply, "vertex", "alpha", alpha_cb, NULL, 0);

    ply_read(myply);

    int lenVect=vectX.size();
    std::cout<<"taille vecteur : "<<vectX.size()<<std::endl;

    // Loop i on all elements
    for(int i=0; i<lenVect;i++)
    {
        std::vector<float>newVector=convertRGBtoTSL(vectR[i],vectG[i],vectB[i]);
        if     (((newVector[0]>=win->mhuemin1 && newVector[0]<=win->mhuemax1) || (newVector[0]>=win->mhuemin2 && newVector[0]<=win->mhuemax2)) &&
                (newVector[1]>=win->msatmin && newVector[1]<=win->msatmax) &&
                (newVector[2]>=win->mlightmin && newVector[2]<=win->mlightmax))
        {
            std::cout<<"In RVB : "<<vectR[i]<<" "<<vectG[i]<<" "<<vectB[i]<<std::endl;
            std::cout<<"In TSL : "<<newVector[0]<<" "<<newVector[1]<<" "<<newVector[2]<<std::endl;
            vectXin.push_back(vectX[i]);
            vectYin.push_back(vectY[i]);
            vectZin.push_back(vectZ[i]);
            vectRin.push_back(vectR[i]);
            vectGin.push_back(vectG[i]);
            vectBin.push_back(vectB[i]);
            vectAin.push_back(vectA[i]);
            //std::cout<<"In : "<<vectXin[i]<<" "<<vectYin[i]<<" "<<vectZin[i]<<" "<<vectRin[i]<<" "<<vectGin[i]<<" "<<vectBin[i]<<" "<<std::endl;


        }
        else
        {
            std::cout<<"\nOut RVB : "<<vectR[i]<<" "<<vectG[i]<<" "<<vectB[i]<<std::endl;
            std::cout<<"Out TSL : "<<newVector[0]<<" "<<newVector[1]<<" "<<newVector[2]<<std::endl;
            vectXout.push_back(vectX[i]);
            vectYout.push_back(vectY[i]);
            vectZout.push_back(vectZ[i]);
            vectRout.push_back(vectR[i]);
            vectGout.push_back(vectG[i]);
            vectBout.push_back(vectB[i]);
            vectAout.push_back(vectA[i]);
            //std::cout<<"Out : "<<vectXout[i]<<" "<<vectYout[i]<<" "<<vectZout[i]<<" "<<vectRout[i]<<" "<<vectGout[i]<<" "<<vectBout[i]<<" \n"<<std::endl;

        }
    }

    writeply(name2, name3);

    ply_close(myply);
}



void writeply(const char* name2, const char* name3)
{

    // Opens files
    std::ofstream filteredFile(name2, std::ios::out | std::ios::trunc);
    std::ofstream remainingFile(name3, std::ios::out | std::ios::trunc);

    // If opening is a success
    if(filteredFile && remainingFile)
    {
        filteredFile<<"ply\n"<<"format ascii 1.0\n"<<"element vertex "<<vectXin.size()<<"\n";
        filteredFile<<"property float x\n"<<"property float y\n"<<"property float z\n"<<"property float red\n"<<"property float green\n"<<"property float blue\n"<<"property float alpha\n";
        filteredFile<<"end header\n";

        for(int i=0;i<vectXin.size();i++)
        {
            filteredFile<<vectXin[i]<<" "<<vectYin[i]<<" "<<vectZin[i]<<" "<<vectRin[i]<<" "<<vectGin[i]<<" "<<vectBin[i]<<" "<<vectAin[i]<<std::endl;
        }

        remainingFile<<"ply\n"<<"format ascii 1.0\n"<<"element vertex "<<vectXout.size()<<"\n";
        remainingFile<<"property float x\n"<<"property float y\n"<<"property float z\n"<<"property float red\n"<<"property float green\n"<<"property float blue\n"<<"property float alpha\n";
        remainingFile<<"end header\n";

        for(int i=0;i<vectXout.size();i++)
        {
            remainingFile<<vectXout[i]<<" "<<vectYout[i]<<" "<<vectZout[i]<<" "<<vectRout[i]<<" "<<vectGout[i]<<" "<<vectBout[i]<<" "<<vectAout[i]<<std::endl;
        }

        // Closes files
        filteredFile.close();
        remainingFile.close();

     }
}

std::vector<float> convertRGBtoTSL(int R,int G,int B)
{
    //std::cout<<"\n*** MainWindow::convertRGBtoTSL() ***"<<std::endl;

    //determination of maximum among R,G,B

    int Max=std::max(std::max(R,G),B);
    int Min=std::min(std::min(R,G),B);

    //std::cout<<"Max : "<<Max<<std::endl;
    //std::cout<<"Min : "<<Min<<std::endl;

    float mValeur_H;
    float mValeur_S;
    float mValeur_L;

    // Difference Max-Min
    int C=Max-Min;
    //std::cout<<"C = "<<C<<std::endl;

    if (R==255 && G==255 && B==255)
    {
        mValeur_H=0;
        mValeur_S=0;
        mValeur_L=100;
    }
    else if (R==0 && G==0 && B==0){
        mValeur_H=0;
        mValeur_S=0;
        mValeur_L=0;
    }
    else
    {
        // Hue
        // max is red
        if(Max==R && Max!=G && Max!=B){mValeur_H=60*(fmod(abs(float(G-B))/float(C),6));}
        // max is green
        else if(Max==G && Max!=R && Max!=B){mValeur_H=60*(2+(float((B-R)/C)));}
        // max is blue
        else if(Max==B && Max!=G && Max!=R){mValeur_H=60*(4+(float((R-G)/C)));}
        // max is red=blue
        else if(Max==B && Max==R && Max!=G){mValeur_H=60*(4+(float((R-G)/C)));}
        // max is red=green
        else if(Max==G && Max==R && Max!=B){mValeur_H=60*(2+(float((B-R)/C)));}
        // max is blue=green
        else if(Max==G && Max==B && Max!=R){mValeur_H=60*(2+(float((B-R)/C)));}
        // red=green=blue
        else if(Max==G && Max==R && Max==B){mValeur_H=0;}

        //std::cout<<"\nHueRef : "<<mValeur_H<<std::endl;

        // Saturation
        mValeur_S=100*(float(C)/float(Max));
        //std::cout<<"SaturationRef : "<<mValeur_S<<std::endl;


        // Luminosity
        mValeur_L=100*(float(Max)/255);
        //std::cout<<"LuminosityRef : "<<mValeur_L<<std::endl;
    }



    // return a vector with HSL float values
    std::vector<float> vectorHSL {mValeur_H,mValeur_S,mValeur_L};


    return vectorHSL;
}

