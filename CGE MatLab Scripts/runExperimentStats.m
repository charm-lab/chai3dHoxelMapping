% FingerMapping Experiments Statistical Analysis
function runExperimentStats()
% Needs to run fingerMappingDataPlotting_v2.m 1st and select 
% "Run Section" in order to pull data from workspace
%% Statistcal Tests v2 -- N-Way ANOVA
    % 2-way ANOVA
    % p = anovan(y, group)
    showStats = "off";
    format long; close all;
    fontSize = 10; width = 1700; height = 1000;
    
    interactableCompare = true;

    % Groups:
    %Mapping group:
    N = numSubjects*numTrialsPerMapping;
    mappings(1     : N,   1) = {'1'};
    mappings(N+1   : 2*N, 1) = {'2'};
    mappings(2*N+1 : 3*N, 1) = {'3'};
    mappings(3*N+1 : 4*N, 1) = {'4'};
    mappings(4*N+1 : 5*N, 1) = {'5'};

    %Convert matrix of subject results to column vectors
    %CompletionTime Mapping 1 visCube group
    yCT_visMap1 = reshape(completionTimeMapping1(:,1:numSubjects),[],1);    
    %CompletionTime Mapping 2 visCube group
    yCT_visMap2 = reshape(completionTimeMapping2(:,1:numSubjects),[],1);    
    %CompletionTime Mapping 3 visCube group
    yCT_visMap3 = reshape(completionTimeMapping3(:,1:numSubjects),[],1);
    %CompletionTime Mapping 4 visCube group
    yCT_visMap4 = reshape(completionTimeMapping4(:,1:numSubjects),[],1);
    %CompletionTime Mapping 5 visCube group
    yCT_visMap5 = reshape(completionTimeMapping5(:,1:numSubjects),[],1);
    
    %IndexPathLength Mapping 1 visCube group
    yIPL_visMap1 = reshape(indexPathLengthMapping1(:,1:numSubjects),[],1);
    %IndexPathLength Mapping 2 visCube group
    yIPL_visMap2 = reshape(indexPathLengthMapping2(:,1:numSubjects),[],1);
    %IndexPathLength Mapping 3 visCube group
    yIPL_visMap3 = reshape(indexPathLengthMapping3(:,1:numSubjects),[],1);
    %IndexPathLength Mapping 4 visCube group
    yIPL_visMap4 = reshape(indexPathLengthMapping4(:,1:numSubjects),[],1);
    %IndexPathLength Mapping 5 visCube group
    yIPL_visMap5 = reshape(indexPathLengthMapping5(:,1:numSubjects),[],1);
    
    %ThumbPathLength Mapping 1 visCube group
    yTPL_visMap1 = reshape(thumbPathLengthMapping1(:,1:numSubjects),[],1);
    %ThumbPathLength Mapping 2 visCube group
    yTPL_visMap2 = reshape(thumbPathLengthMapping2(:,1:numSubjects),[],1);
    %ThumbPathLength Mapping 3 visCube group
    yTPL_visMap3 = reshape(thumbPathLengthMapping3(:,1:numSubjects),[],1);
    %ThumbPathLength Mapping 4 visCube group
    yTPL_visMap4 = reshape(thumbPathLengthMapping4(:,1:numSubjects),[],1);
    %ThumbPathLength Mapping 5 visCube group
    yTPL_visMap5 = reshape(thumbPathLengthMapping5(:,1:numSubjects),[],1);
    
    %BoxPathLength Mapping 1 visCube group
    yBPL_visMap1 = reshape(boxPathLengthMapping1(:,1:numSubjects),[],1);
    %BoxPathLength Mapping 2 visCube group
    yBPL_visMap2 = reshape(boxPathLengthMapping2(:,1:numSubjects),[],1);
    %BoxPathLength Mapping 3 visCube group
    yBPL_visMap3 = reshape(boxPathLengthMapping3(:,1:numSubjects),[],1);
    %BoxPathLength Mapping 4 visCube group
    yBPL_visMap4 = reshape(boxPathLengthMapping4(:,1:numSubjects),[],1);
    %BoxPathLength Mapping 5 visCube group
    yBPL_visMap5 = reshape(boxPathLengthMapping5(:,1:numSubjects),[],1);
    
    %IndexShearForceMag Mapping 1 visCube group
    yISF_visMap1 = reshape(meanIndexShearForceMapping1(:,1:numSubjects),[],1);
    %IndexShearForceMag Mapping 2 visCube group
    yISF_visMap2 = reshape(meanIndexShearForceMapping2(:,1:numSubjects),[],1);
    %IndexShearForceMag Mapping 3 visCube group
    yISF_visMap3 = reshape(meanIndexShearForceMapping3(:,1:numSubjects),[],1);
    %IndexShearForceMag Mapping 4 visCube group
    yISF_visMap4 = reshape(meanIndexShearForceMapping4(:,1:numSubjects),[],1);
    %IndexShearForceMag Mapping 5 visCube group
    yISF_visMap5 = reshape(meanIndexShearForceMapping5(:,1:numSubjects),[],1);
    
    %IndexNormalForceMag Mapping 1 visCube group
    yINF_visMap1 = reshape(meanIndexNormalForceMapping1(:,1:numSubjects),[],1);
    %IndexNormalForceMag Mapping 2 visCube group
    yINF_visMap2 = reshape(meanIndexNormalForceMapping2(:,1:numSubjects),[],1);
    %IndexNormalForceMag Mapping 3 visCube group
    yINF_visMap3 = reshape(meanIndexNormalForceMapping3(:,1:numSubjects),[],1);
    %IndexNormalForceMag Mapping 4 visCube group
    yINF_visMap4 = reshape(meanIndexNormalForceMapping4(:,1:numSubjects),[],1);
    %IndexNormalForceMag Mapping 5 visCube group
    yINF_visMap5 = reshape(meanIndexNormalForceMapping5(:,1:numSubjects),[],1);
    
    %ThumbShearForceMag Mapping 1 visCube group
    yTSF_visMap1 = reshape(meanThumbShearForceMapping1(:,1:numSubjects),[],1);
    %ThumbShearForceMag Mapping 2 visCube group
    yTSF_visMap2 = reshape(meanThumbShearForceMapping2(:,1:numSubjects),[],1);
    %ThumbShearForceMag Mapping 3 visCube group
    yTSF_visMap3 = reshape(meanThumbShearForceMapping3(:,1:numSubjects),[],1);
    %ThumbShearForceMag Mapping 4 visCube group
    yTSF_visMap4 = reshape(meanThumbShearForceMapping4(:,1:numSubjects),[],1);
    %ThumbShearForceMag Mapping 5 visCube group
    yTSF_visMap5 = reshape(meanThumbShearForceMapping5(:,1:numSubjects),[],1);
    
    %ThumbNormalForceMag Mapping 1 visCube group
    yTNF_visMap1 = reshape(meanThumbNormalForceMapping1(:,1:numSubjects),[],1);
    %ThumbNormalForceMag Mapping 2 visCube group
    yTNF_visMap2 = reshape(meanThumbNormalForceMapping2(:,1:numSubjects),[],1);
    %ThumbNormalForceMag Mapping 3 visCube group
    yTNF_visMap3 = reshape(meanThumbNormalForceMapping3(:,1:numSubjects),[],1);
    %ThumbNormalForceMag Mapping 4 visCube group
    yTNF_visMap4 = reshape(meanThumbNormalForceMapping4(:,1:numSubjects),[],1);
    %ThumbNormalForceMag Mapping 5 visCube group
    yTNF_visMap5 = reshape(meanThumbNormalForceMapping5(:,1:numSubjects),[],1);
    
%     %BoxDrops Mapping 1 visCube group
%     yBD_visMap1 = reshape(boxDropsMapping1(:,1:numSubjects),[],1);
%     %BoxDrops Mapping 2 visCube group
%     yBD_visMap2 = reshape(boxDropsMapping2(:,1:numSubjects),[],1);
%     %BoxDrops Mapping 3 visCube group
%     yBD_visMap3 = reshape(boxDropsMapping3(:,1:numSubjects),[],1);
%     %BoxDrops Mapping 4 visCube group
%     yBD_visMap4 = reshape(boxDropsMapping4(:,1:numSubjects),[],1);
%     %BoxDrops Mapping 5 visCube group
%     yBD_visMap5 = reshape(boxDropsMapping5(:,1:numSubjects),[],1);
    
    %boxRMSXMapping Mapping 1
    yRMSX_Map1 = reshape(boxRMSXMapping1(:,1:numSubjects),[],1);
    %boxRMSXMapping Mapping 2
    yRMSX_Map2 = reshape(boxRMSXMapping2(:,1:numSubjects),[],1);
    %boxRMSXMapping Mapping 3
    yRMSX_Map3 = reshape(boxRMSXMapping3(:,1:numSubjects),[],1);
    %boxRMSXMapping Mapping 4
    yRMSX_Map4 = reshape(boxRMSXMapping4(:,1:numSubjects),[],1);
    %boxRMSXMapping Mapping 5
    yRMSX_Map5 = reshape(boxRMSXMapping5(:,1:numSubjects),[],1);

    %boxRMSYMapping Mapping 1
    yRMSY_Map1 = reshape(boxRMSYMapping1(:,1:numSubjects),[],1);
    %boxRMSYMapping Mapping 2
    yRMSY_Map2 = reshape(boxRMSYMapping2(:,1:numSubjects),[],1);
    %boxRMSYMapping Mapping 3
    yRMSY_Map3 = reshape(boxRMSYMapping3(:,1:numSubjects),[],1);
    %boxRMSYMapping Mapping 4
    yRMSY_Map4 = reshape(boxRMSYMapping4(:,1:numSubjects),[],1);
    %boxRMSYMapping Mapping 5
    yRMSY_Map5 = reshape(boxRMSYMapping5(:,1:numSubjects),[],1);

    %percentTimeHigh Mapping 1
    yPTH_Map1 = reshape(manipForceBoolMapping1(:,1:numSubjects),[],1);
    %percentTimeHigh Mapping 2
    yPTH_Map2 = reshape(manipForceBoolMapping2(:,1:numSubjects),[],1);
    %percentTimeHigh Mapping 3
    yPTH_Map3 = reshape(manipForceBoolMapping3(:,1:numSubjects),[],1);
    %percentTimeHigh Mapping 4
    yPTH_Map4 = reshape(manipForceBoolMapping4(:,1:numSubjects),[],1);
    %percentTimeHigh Mapping 5
    yPTH_Map5 = reshape(manipForceBoolMapping5(:,1:numSubjects),[],1);

    %Find p-values
    %vertically concatenate columns of the same metric
    yCT = [yCT_visMap1; yCT_visMap2; yCT_visMap3; yCT_visMap4; yCT_visMap5];
    [p_CompletionTime, ~, stats_CompletionTime ] = anovan(yCT, {mappings},...
        "Model","interaction", "Varnames","mappings", "display",showStats);

    yIPL = [yIPL_visMap1; yIPL_visMap2; yIPL_visMap3; yIPL_visMap4; yIPL_visMap5];
    [p_IndexPathLength, ~, stats_IndexPathLength] = anovan(yIPL, {mappings},...
        "Model","interaction", "Varnames","mappings", "display",showStats);

    yTPL = [yTPL_visMap1; yTPL_visMap2; yTPL_visMap3; yTPL_visMap4; yTPL_visMap5];
    [p_ThumbPathLength, ~, stats_ThumbPathLength] = anovan(yTPL, {mappings},...
        "Model","interaction", "Varnames","mappings","display",showStats);

    yBPL = [yBPL_visMap1; yBPL_visMap2; yBPL_visMap3; yBPL_visMap4; yBPL_visMap5];
    %     p_BoxPathLength = anovan(yBPL, {mappings})
    [p_BoxPathLength, ~, stats_BoxPathLength] = anovan(yBPL, {mappings},...
        "Model","interaction", "Varnames","mappings", "display",showStats);
    
    yISF = [yISF_visMap1; yISF_visMap2; yISF_visMap3; yISF_visMap4; yISF_visMap5];
    [p_IndexShearForce, ~, stats_IndexShearForce] = anovan(yISF, {mappings},...
        "Model","interaction", "Varnames","mappings","display",showStats);

    yINF = [yINF_visMap1; yINF_visMap2; yINF_visMap3; yINF_visMap4; yINF_visMap5];
    [p_IndexNormalForce, ~, stats_IndexNormalForce] = anovan(yINF, {mappings},...
        "Model","interaction", "Varnames", "mappings", "display",showStats);

    yTSF = [yTSF_visMap1; yTSF_visMap2; yTSF_visMap3; yTSF_visMap4; yTSF_visMap5];
    [p_ThumbShearForce, ~, stats_ThumbShearForce] = anovan(yTSF, {mappings},...
        "Model","interaction", "Varnames", "mappings", "display",showStats);

    yTNF = [yTNF_visMap1; yTNF_visMap2; yTNF_visMap3; yTNF_visMap4; yTNF_visMap5];
    [p_ThumbNormalForce, ~, stats_ThumbNormalForce] = anovan(yTNF, {mappings},...
        "Model","interaction", "Varnames", "mappings", "display",showStats);

%     yBD = [yBD_visMap1; yBD_visMap2; yBD_visMap3; yBD_visMap4; yBD_visMap5];
%     [p_BoxDrops, ~, stats_BoxDrops] = anovan(yBD,  {mappings},...
%         "Model","interaction", "Varnames", "mappings", "display",showStats);

    yRMSX = [yRMSX_Map1; yRMSX_Map2; yRMSX_Map3; yRMSX_Map4; yRMSX_Map5];
    [p_BoxRMSX, ~, stats_BoxRMSX] = anovan(yRMSX, {mappings},...
        "Model","interaction", "Varnames", "mappings", "display",showStats);

    yRMSY = [yRMSY_Map1; yRMSY_Map2; yRMSY_Map3; yRMSY_Map4; yRMSY_Map5];
    [p_BoxRMSY, ~, stats_BoxRMSY] = anovan(yRMSY, {mappings},...
        "Model","interaction", "Varnames", "mappings", "display",showStats);

    yPTH = [yPTH_Map1; yPTH_Map2; yPTH_Map3; yPTH_Map4; yPTH_Map5];
    [p_PTH, ~, stats_PTH] = anovan(yPTH, {mappings},...
        "Model","interaction", "Varnames", "mappings", "display",showStats);

    %Compare signifcance with control group only:
    if(interactableCompare == false)
        close all;
        figure(1)
        %Completion Time
        subplot(3,3,1)    
        compCT = multcompare(stats_CompletionTime, "CriticalValueType", "dunnett", ...
            "ControlGroup", 5, "Alpha", 0.05);
        title("Completion Time -- Control")
        improvePlot_v2(false, true, fontSize, width, height)
    
        %Index Path Length   
        subplot(3,3,2)    
        compIPL = multcompare(stats_IndexPathLength, "CriticalValueType", "dunnett", ...
            "ControlGroup", 5, "Alpha", 0.05);
        title("Index Path Length -- Control")
        improvePlot_v2(false, true, fontSize, width, height)
    
        %Thumb Path Length
        subplot(3,3,3)    
        compTPL = multcompare(stats_ThumbPathLength, "CriticalValueType", "dunnett", ...
            "ControlGroup", 5, "Alpha", 0.05);
        title("Thumb Path Length -- Control")
        improvePlot_v2(false, true, fontSize, width, height)
    
        %Box Path Length
        subplot(3,3,4)    
        compBPL = multcompare(stats_BoxPathLength, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        title("Box Path Length -- Control")
        improvePlot_v2(false, true, fontSize, width, height)    
    
        %Index Shear Force
        subplot(3,3,5)        
        compISF = multcompare(stats_IndexShearForce, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        title("Index Shear Force -- Control")
        improvePlot_v2(false, true, fontSize, width, height)    
    
        %Index Normal Force
        subplot(3,3,6)    
        compINF = multcompare(stats_IndexNormalForce, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        title("Index Normal Force -- Control")
        improvePlot_v2(false, true, fontSize, width, height)
        
        %Thumb Shear Force
        subplot(3,3,7)    
        compTSF = multcompare(stats_ThumbShearForce, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        title("Thumb Shear Force -- Control")
        improvePlot_v2(false, true, fontSize, width, height)           
    
        %Thumb Normal Force
        subplot(3,3,8)    
        compTNF = multcompare(stats_ThumbNormalForce, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        improvePlot_v2(false, true, fontSize, width, height) 
        title("Thumb Normal Force -- Control")
        
%         %Box Drops
%         subplot(3,3,9)    
%         %Compare signifcance with control group only:
%         compBD = multcompare(stats_BoxDrops, "CriticalValueType", "dunnett", ...
%             "ControlGroup", 5, "Alpha", 0.05);        
%         improvePlot_v2(false, true, fontSize, width, height)
%         title("BoxDrops -- Control")

        %Box RMSX
        figure(2)
        subplot(1,3,1)    
        compRMSX = multcompare(stats_BoxRMSX, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        improvePlot_v2(false, true, fontSize, width, height) 
        title("Box RMS X -- Control")

        %Box RMSY
        subplot(1,3,2)    
        compRMSY = multcompare(stats_BoxRMSY, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        improvePlot_v2(false, true, fontSize, width, height) 
        title("Box RMS Y -- Control")

        %Percent Too High
        subplot(1,3,3)    
        compPTH = multcompare(stats_PTH, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        improvePlot_v2(false, true, fontSize, width, height) 
        title("PercentTooHigh -- Control")
    
        %Tables with Mapping 5 as Control:    
        tbl_CompletionTime = array2table(compCT,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_IndexPathLength = array2table(compIPL,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_ThumbPathLength = array2table(compTPL,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_BoxPathLength = array2table(compBPL,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_IndexShearForce = array2table(compISF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_IndexNormalForce = array2table(compINF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_ThumbShearForce = array2table(compTSF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_ThumbNormalForce = array2table(compTNF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
%         tbl_BoxDrops = array2table(compBD,"VariableNames", ...
%         ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
%     

        tbl_BoxRMSX = array2table(compRMSX,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])

        tbl_BoxRMSY = array2table(compRMSY,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])

        tbl_PTH = array2table(compPTH,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])

    %Interactable comparison with any group:
    else    
        close all;
        figure(1)
    
        subplot(3,3,1)
        compCT = multcompare(stats_CompletionTime);
        title("Completion Time -- Interactable")
        improvePlot_v2(false, true, fontSize, width, height)
    
        subplot(3,3,2)
        compIPL = multcompare(stats_IndexPathLength);
        title("Index Path Length -- Interactable")
        improvePlot_v2(false, true, fontSize, width, height)
    
        subplot(3,3,3)
        compTPL = multcompare(stats_ThumbPathLength);
        title("Thumb Path Length -- Interactable")
        improvePlot_v2(false, true, fontSize, width, height)
    
        subplot(3,3,4)
        compBPL = multcompare(stats_BoxPathLength);
        title("Box Path Length -- Interactable")
        improvePlot_v2(false, true, fontSize, width, height)
    
        subplot(3,3,5)
        compISF = multcompare(stats_IndexShearForce);
        title("Index Shear Force -- Interactable")
        improvePlot_v2(false, true, fontSize, width, height)
    
        subplot(3,3,6)
        compINF = multcompare(stats_IndexNormalForce);
        title("Index Normal Force -- Interactable")
        improvePlot_v2(false, true, fontSize, width, height)
    
        subplot(3,3,7)
        compTSF = multcompare(stats_ThumbShearForce);
        title("Thumb Shear Force -- Interactable")
        improvePlot_v2(false, true, fontSize, width, height)
    
        subplot(3,3,8)
        compTNF = multcompare(stats_ThumbNormalForce);
        title("Thumb Normal Force -- Interactable")
        improvePlot_v2(false, true, fontSize, width, height)
    
%         subplot(3,3,9)
%         compBD = multcompare(stats_BoxDrops);
%         title("BoxDrops -- Interactable")
%         improvePlot_v2(false, true, fontSize, width, height)    
        
        %Box RMSX
        figure(2)
        subplot(1,3,1)    
        compRMSX = multcompare(stats_BoxRMSX);
        improvePlot_v2(false, true, fontSize, width, height) 
        title("Box RMS X -- Interactable")

        %Box RMSY
        subplot(1,3,2)    
        compRMSY = multcompare(stats_BoxRMSY);
        improvePlot_v2(false, true, fontSize, width, height) 
        title("Box RMS Y -- Interactable")

        %Percent Too High
        subplot(1,3,3)    
        compPTH = multcompare(stats_PTH);
        improvePlot_v2(false, true, fontSize, width, height) 
        title("PercentTooHigh -- Interactable")

        %Tables with Variable Control:    
        tbl_CompletionTime = array2table(compCT,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_IndexPathLength = array2table(compIPL,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_ThumbPathLength = array2table(compTPL,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_BoxPathLength = array2table(compBPL,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_IndexShearForce = array2table(compISF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_IndexNormalForce = array2table(compINF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_ThumbShearForce = array2table(compTSF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_ThumbNormalForce = array2table(compTNF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
%         tbl_BoxDrops = array2table(compBD,"VariableNames", ...
%         ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])

        tbl_BoxRMSX = array2table(compRMSX,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])

        tbl_BoxRMSY = array2table(compRMSY,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])

        tbl_PTH = array2table(compPTH,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    end

    if (showStats == "on")
        p_CompletionTime
        p_IndexPathLength
        p_ThumbPathLength
        p_BoxPathLength
        p_IndexShearForce
        p_IndexNormalForce
        p_ThumbShearForce
        p_ThumbNormalForce
%         p_BoxDrops
        p_BoxRMSX
        p_BoxRMSY
        p_PTH
    end       
end