%   Creates an errorbar plot with the visible and invible cube group data
%   Also assumes subjects are sorted columnwise:
%   [visibleCube | invisibleCube]
%   in the SubjectData cell

function [visCubeErrorBarPlot, invisCubeErrorBarPlot, visCubeMeanVals,...
    invisCubeMeanVals, visCubeStdVals, invisCubeStdVals] = ...
    createErrorBarPlot_unevenGroups(map1, map2, map3, map4, map5,...
    visGroupSubjectNum, invisGroupSubjectNum,...
    plotTitle, xAxisLabel, yAxisLabel)

%Pull variables from workspace
numSubjects = evalin('base','numSubjects');
visCubeColor = evalin('base','visCubeColor');
invisCubeColor = evalin('base','invisCubeColor');
jitterVal = evalin('base','jitterVal');
numMappings = evalin('base','numMappings');
plotMarker = evalin('base','plotMarker');
markerSize = evalin('base', 'markerSize');
showIndivSubData = evalin('base', 'showIndivSubData');

%Calculate means and standard deviations for Vis Cube Subjects
cols1 = visGroupSubjectNum; %visCubeSubjects indices -- not necessarily sequential
mapping1_mean = mean(mean(map1(:,cols1))); mapping1_std = mean(std(map1(:,cols1)));
mapping2_mean = mean(mean(map2(:,cols1))); mapping2_std = mean(std(map2(:,cols1)));
mapping3_mean = mean(mean(map3(:,cols1))); mapping3_std = mean(std(map3(:,cols1)));
mapping4_mean = mean(mean(map4(:,cols1))); mapping4_std = mean(std(map4(:,cols1)));
mapping5_mean = mean(mean(map5(:,cols1))); mapping5_std = mean(std(map5(:,cols1)));

visCubeMeanVals = [mapping1_mean, mapping2_mean, mapping3_mean, mapping4_mean, mapping5_mean];
visCubeStdVals = [mapping1_std, mapping2_std, mapping3_std, mapping4_std, mapping5_std];

%Calculate means and standard deviations for invis Cube Subjects
cols2 = invisGroupSubjectNum; %invisCubeSubjects indices -- not necessarily sequential
mapping1_mean = mean(mean(map1(:,cols2))); mapping1_std = mean(std(map1(:,cols2)));
mapping2_mean = mean(mean(map2(:,cols2))); mapping2_std = mean(std(map2(:,cols2)));
mapping3_mean = mean(mean(map3(:,cols2))); mapping3_std = mean(std(map3(:,cols2)));
mapping4_mean = mean(mean(map4(:,cols2))); mapping4_std = mean(std(map4(:,cols2)));
mapping5_mean = mean(mean(map5(:,cols2))); mapping5_std = mean(std(map5(:,cols2)));

invisCubeMeanVals = [mapping1_mean, mapping2_mean, mapping3_mean, mapping4_mean, mapping5_mean];
invisCubeStdVals = [mapping1_std, mapping2_std, mapping3_std, mapping4_std, mapping5_std];

%Vis Cube Subjects Plots
visCubeErrorBarPlot = errorbar((1:numMappings)-jitterVal,...
    visCubeMeanVals, visCubeStdVals, plotMarker, ...
    "MarkerFaceColor", str2num(visCubeColor), ...
    "Color", str2num(visCubeColor), "MarkerSize", markerSize, "LineWidth", 2);
hold on;

if (showIndivSubData == true)
    %Reshape the matrices that contain the number of drops sorted by
    %mapping so that they are a single column lined up my the mapping
    %number to be plotted
    indivVisSubsByMap = [reshape(map1(:,cols1), [], 1),...
                         reshape(map2(:,cols1), [], 1),...
                         reshape(map3(:,cols1), [], 1),...
                         reshape(map4(:,cols1), [], 1),...
                         reshape(map5(:,cols1), [], 1)];
    x = repmat((1:numMappings)-jitterVal,length(indivVisSubsByMap),1)
    swarmchart(x, indivVisSubsByMap, "*", ...
     "Color", str2num(visCubeColor), "LineWidth", 1);
end


%Invis Cube Subjects Plots
invisCubeErrorBarPlot = errorbar((1:numMappings)+jitterVal,...
    invisCubeMeanVals, invisCubeStdVals, plotMarker,...
    "Color", str2num(invisCubeColor), "MarkerSize", markerSize, "LineWidth", 2);

%Plot Details
xlim([0 6]); tickLabels = ["1", "2", "3", "4", "Control"];
xticks(0:6); set(gca,'xTick', [1:numMappings],'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);
hold off;

end


