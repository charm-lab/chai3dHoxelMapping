% FingerMapping Experiments Statistical Analysis
function runHMEStats()
% Needs to run fingerMappingDataPlotting_v2.m 1st and select 
% "Run Section" in order to pull data from workspace
%% Statistcal Tests v2 -- N-Way ANOVA
    % 2-way ANOVA
    % p = anovan(y, group)
    showStats = "off";
    format short
    % Groups:
    %Mapping group:
    mappings(1:10,1) = {'1'};
    mappings(11:20,1) = {'2'};
    mappings(21:30,1) = {'3'};
    mappings(31:40,1) = {'4'};
    mappings(41:50,1) = {'5'};
    
    %Visibility condition
    visibiltyCondition(1:10,1)   = {'visibleCube'};
    visibiltyCondition(11:20,1) = {'visibleCube'};
    visibiltyCondition(21:30,1) = {'visibleCube'};
    visibiltyCondition(31:40,1) = {'visibleCube'};
    visibiltyCondition(41:50,1) = {'visibleCube'};
    
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
    
    %BoxDrops Mapping 1 visCube group
    yBD_visMap1 = reshape(boxDropsMapping1(:,1:numSubjects),[],1);
    %BoxDrops Mapping 2 visCube group
    yBD_visMap2 = reshape(boxDropsMapping2(:,1:numSubjects),[],1);
    %BoxDrops Mapping 3 visCube group
    yBD_visMap3 = reshape(boxDropsMapping3(:,1:numSubjects),[],1);
    %BoxDrops Mapping 4 visCube group
    yBD_visMap4 = reshape(boxDropsMapping4(:,1:numSubjects),[],1);
    %BoxDrops Mapping 5 visCube group
    yBD_visMap5 = reshape(boxDropsMapping5(:,1:numSubjects),[],1);
    
    %Find p-values
    %vertically concatenate columns of the same metric
    %Completion Time
    yCT = [yCT_visMap1; yCT_visMap2; yCT_visMap3; yCT_visMap4; yCT_visMap5];
    %     p_CompletionTime = anovan(yCT, {mappings})
    [p_CompletionTime, tbl_CompletionTime , stats_CompletionTime ] = ...
        anovan(yCT,  {mappings},...
        "Model","interaction", "Varnames",["mappings"],...
        "display",showStats);
    figure(1)
    multcompare(stats_CompletionTime, "CriticalValueType", "dunnett", ...
        "ControlGroup", 5, "Alpha", 0.05);
    %Interactable comparison with any group:
    %     multcompare(stats_CompletionTime);
    %     title("Completion Time -- Interactable")
    title("Completion Time")
    improvePlot_v2(false, true, 10, 500, 500)

    %Index Path Length
    yIPL = [yIPL_visMap1; yIPL_visMap2; yIPL_visMap3; yIPL_visMap4; yIPL_visMap5];
    %     p_IndexPathLength = anovan(yIPL, {mappings})
    [p_IndexPathLength, tbl_IndexPathLength, stats_IndexPathLength] = ...
        anovan(yIPL,  {mappings},...
        "Model","interaction", "Varnames",["mappings"],...
        "display",showStats);   
    figure(2)
    multcompare(stats_IndexPathLength, "CriticalValueType", "dunnett", ...
        "ControlGroup", 5, "Alpha", 0.05);
    %Interactable comparison with any group:
    %     multcompare(stats_IndexPathLength);
    %     title("Index Path Length -- Interactable")
    title("Index Path Length")
    improvePlot_v2(false, true, 10, 500, 500)

    %Thumb Path Length
    yTPL = [yTPL_visMap1; yTPL_visMap2; yTPL_visMap3; yTPL_visMap4; yTPL_visMap5];
    %     p_ThumbPathLength = anovan(yTPL, {mappings})
    [p_ThumbPathLength, tbl_ThumbPathLength, stats_ThumbPathLength] = ...
        anovan(yTPL,  {mappings},...
        "Model","interaction", "Varnames",["mappings"],...
        "display",showStats);    
    figure(3)
    multcompare(stats_ThumbPathLength, "CriticalValueType", "dunnett", ...
        "ControlGroup", 5, "Alpha", 0.05);
    %Interactable comparison with any group:
    %     multcompare(stats_ThumbPathLength);
    %     title("Thumb Path Length -- Interactable")
    title("Thumb Path Length")
    improvePlot_v2(false, true, 10, 500, 500)

    %Box Path Length
    yBPL = [yBPL_visMap1; yBPL_visMap2; yBPL_visMap3; yBPL_visMap4; yBPL_visMap5];
    %     p_BoxPathLength = anovan(yBPL, {mappings})
    [p_BoxPathLength, tbl_BoxPathLength, stats_BoxPathLength] = ...
        anovan(yBPL,  {mappings},...
        "Model","interaction", "Varnames",["mappings"],...
        "display",showStats);
    figure(4)
    multcompare(stats_BoxPathLength, "CriticalValueType", "dunnett", ...
        "ControlGroup", 5, "Alpha", 0.05);
    %Interactable comparison with any group:
    %     multcompare(stats_BoxPathLength);
    %     title("Box Path Length -- Interactable")
    title("Box Path Length")
    improvePlot_v2(false, true, 10, 500, 500)

    %Index Shear Force
    yISF = [yISF_visMap1; yISF_visMap2; yISF_visMap3; yISF_visMap4; yISF_visMap5];
    %     p_IndexShearForce = anovan(yISF, {mappings})
    [p_IndexShearForce, tbl_IndexShearForce, stats_IndexShearForce] = ...
        anovan(yISF,  {mappings},...
        "Model","interaction", "Varnames",["mappings"],...
        "display",showStats);    
    figure(5)
    multcompare(stats_IndexShearForce, "CriticalValueType", "dunnett", ...
        "ControlGroup", 5, "Alpha", 0.05);
    %Interactable comparison with any group:
    %     multcompare(stats_IndexShearForce);
    %     title("Index Shear Force -- Interactable")
    title("Index Shear Force")
    improvePlot_v2(false, true, 10, 500, 500)

    %Index Normal Force
    yINF = [yINF_visMap1; yINF_visMap2; yINF_visMap3; yINF_visMap4; yINF_visMap5];
    %     p_IndexNormalForce = anovan(yINF, {mappings})
    [p_IndexNormalForce, tbl_IndexNormalForce, stats_IndexNormalForce] = ...
        anovan(yINF,  {mappings},...
        "Model","interaction", "Varnames",["mappings"],...
        "display",showStats);
    figure(6)
    multcompare(stats_IndexNormalForce, "CriticalValueType","dunnett", ...
        "ControlGroup", 5, "Alpha", 0.05);
    %Interactable comparison with any group:
    %     multcompare(stats_IndexNormalForce);
    %     title("Index Normal Force -- Interactable")
    title("Index Normal Force")
    improvePlot_v2(false, true, 10, 500, 500)

    %Thumb Shear Force
    yTSF = [yTSF_visMap1; yTSF_visMap2; yTSF_visMap3; yTSF_visMap4; yTSF_visMap5];
    %     p_ThumbShearForce = anovan(yTSF, {mappings})
    [p_ThumbShearForce, tbl_ThumbShearForce, stats_ThumbShearForce] = ...
        anovan(yTSF,  {mappings},...
        "Model","interaction", "Varnames",["mappings"],...
        "display",showStats);
    figure(7)
    %subplot(1,3,1)
    %Compare signifcance with control group only:
    multcompare(stats_ThumbShearForce, "CriticalValueType", "dunnett", ...
        "ControlGroup", 5, "Alpha", 0.05);
    %Interactable comparison with any group:
    %     multcompare(stats_ThumbShearForce);
    %     title("Thumb Shear Force -- Interactable")
    title("Thumb Shear Force")
    improvePlot_v2(false, true, 10, 500, 500) 

    %Thumb Normal Force
    yTNF = [yTNF_visMap1; yTNF_visMap2; yTNF_visMap3; yTNF_visMap4; yTNF_visMap5];
    %     p_ThumbNormalForce = anovan(yTNF, {mappings})
    [p_ThumbNormalForce, tbl_ThumbNormalForce, stats_ThumbNormalForce] = ...
        anovan(yTNF,  {mappings},...
        "Model","interaction", "Varnames",["mappings"],...
        "display",showStats); 
    figure(8)
    %Compare signifcance with control group only:
    multcompare(stats_ThumbNormalForce, "CriticalValueType", "dunnett", ...         
        "ControlGroup", 5, "Alpha", 0.05); 
    %Interactable comparison with any group:
    %   multcompare(stats_ThumbNormalForce); 
    %   title("Thumb Normal Force -- Interactable") 
    improvePlot_v2(false, true, 10, 500, 500) 
    title("Thumb Normal Force -- Control")
    %subplot(1,3,2)  

    %Box Drops
    yBD = [yBD_visMap1; yBD_visMap2; yBD_visMap3; yBD_visMap4; yBD_visMap5];
    %     p_BoxDrops = anovan(yBD, {mappings})
    [p_BoxDrops, ~ , stats_BoxDrops] = ...
        anovan(yCT,  {mappings},...
        "Model","interaction", "Varnames",["mappings"],...
        "display",showStats);
    figure(9)
    %Compare signifcance with control group only:
    compBD = multcompare(stats_BoxDrops, "CriticalValueType", "dunnett", ...
        "ControlGroup", 5, "Alpha", 0.05); 
    %Interactable comparison with any group:
    %   multcompare(stats_BoxDrops); 
    %   title("BoxDrops -- Interactable")    
    improvePlot_v2(false, true, 10, 500, 500)
    title("BoxDrops -- Control")
    %subplot(1,3,3)
    
    tbl_BoxDrops = array2table(compBD,"VariableNames", ...
    ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])

    if (showStats == "off")
        p_CompletionTime
        p_IndexPathLength
        p_ThumbPathLength
        p_BoxPathLength
        p_IndexShearForce
        p_IndexNormalForce
        p_ThumbShearForce
        p_ThumbNormalForce
        p_BoxDrops
    end
        
end