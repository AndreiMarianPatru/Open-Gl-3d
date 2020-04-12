#include "OBJLoader.h"

vector<Vertex> OBJLoader::LoadOBJ(const string& FolderLocconst, const string& FileLoc, string& AmbiantLoc,
	string& DiffLoc, string& specLoc, string& NormalLoc, vector<glm::uint>& indicies)
{
	string line;
	string MatLibName;
	string MaterialOfMesh;
	string MeshName;
	vector<glm::vec3> VertPositions;
	vector<glm::vec3> VertNormals;
	vector<glm::vec3> VertTextureCoords;
	vector<Vertex> Finalverts;

	std::ifstream file;
	string FileLoc=FolderLocconst+"/"+FileLoc;
	const char* fileNameChar=FileLoc.c_str();
	file.open(fileNameChar,ifstream::in);
	if(file.is_open())
	{
		while(file.good())
		{
			getline(file,line);
			if(line[0]!="#")
			{
				string FirstWord=line.substr(0,line.find(' '));
				if(FirstWord=="mtllib")
				{
					MatLibName=line.substr(line.find(' ')+1,line.find('\n'));
				}
				else if(FirstWord=="o")
				{
					MeshName=line.substr(line.find(' '),line.find('\n'));
				}
				else if(FirstWord=="v")
				{
					string VertValues=line.substr(line.find(' '),line.find('\n'));
					float x,y,z;
					sscanf_s(VertValues.c_str(),"%f %f %f",&x,&y,&z);
					VertPositions.push_back(glm::vec3(x,y,z));
				}
				else if(FirstWord=="vn")
				{
					string VertNormValues=line.substr(line.find(' '),line.find('\n'));
					float x,y,z;
					sscanf_s(VertNormValues.c_str(),"%f %f %f",&x,&y,&z);
					VertNormals.push_back(glm::vec3(x,y,z));
				}
				else if(FirstWord=="vt")
				{
					string VertTexValues=line.substr(line.find(' '),line.find('\n'));
					float x,y,z;
					sscanf_s(VertTexValues.c_str(),"%f %f %f",&x,&y,&z);
					VertTextureCoords.push_back(glm::vec3(x,y,z));
				}
				else if(FirstWord=="usemtl")
				{
					MaterialOfMesh=line.substr(line.find(' ')+1,line.find('\n'));
					LoadMaterial(FolderLocconst+"/"+MatLibName,AmbiantLoc,DiffLoc,specLoc,NormalLoc);
				}
				else if(FirstWord=="f")
				{
					string FaceValues=line.substr(line.find(' '),line.find('\n'));
					Vertex vertInFace[3];
					unsigned int TmpPosition[3],TmpTexCoords[3],TmpNormals[3];
					sscanf_s(FaceValues.c_str()," %d/%d/%d %d/%d/%d %d/%d/%d",
						&TmpPosition[0],&TmpTexCoords[0],&TmpNormals[0],
						&TmpPosition[1],&TmpTexCoords[1],&TmpNormals[1],
						&TmpPosition[2],&TmpTexCoords[2],&TmpNormals[2]);
					vertInFace[0].Position=VertPositions[TmpPosition[0]-1];
					vertInFace[0].TextureCoord=VertTextureCoords[TmpTexCoords[0]-1];
					vertInFace[0].=VertPositions[TmpPosition[0]-1];
					
				}
			}
		}
		
	}
	else
	{
		cerr<<"unable to load text file "<<FolderLocconst+"/"+FileLoc<<endl;
	}
}

void OBJLoader::LoadMaterial(const string& MatLibLoc, string& AmbiantLoc, string& DiffLoc, string& specLoc,
	string& NormalLoc)
{
}
