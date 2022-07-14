.PHONY: clean All Project_Title Project_Build Project_PostBuild

All: Project_Title Project_Build Project_PostBuild

Project_Title:
	@echo "----------Building project:[ fpv_app_umac4 - BuildSet ]----------"

Project_Build:
	@make -r -f fpv_app_umac4.mk -j 8 -C  ./ 

Project_PostBuild:
	@echo Executing Post Build commands ...
	@export CDKPath="D:/C-Sky/CDK" CDK_VERSION="V2.8.8" ProjectPath="D:/Code_management/Taixin-wei/hgSDK-v2.1.0.7-18624/hgSDK-v2.1.0.7-18624/project/" && D:/Code_management/Taixin-wei/hgSDK-v2.1.0.7-18624/hgSDK-v2.1.0.7-18624/project/BuildBIN.sh
	@echo Done


clean:
	@echo "----------Cleaning project:[ fpv_app_umac4 - BuildSet ]----------"

