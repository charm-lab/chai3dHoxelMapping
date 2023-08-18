%   Creates an errorbar plot with trial data
%   Also assumes subjects are sorted columnwise
%   in the SubjectData cell

function [multiExpErrorBarPlot] = createMultiExpErrorBarPlot(meanVals, stdVals,...
    plotTitle, xAxisLabel, yAxisLabel)

% Pull variables from workspace
trainingMap1Color = evalin('base','trainingMap1Color');
testingMap1Color = evalin('base','testingMap1Color');
jitterVal = evalin('base','jitterVal');
numMappings = evalin('base','numMappings');
plotMarker = evalin('base','plotMarker');
markerSize = evalin('base', 'markerSize'); 
numExperimentTypes = evalin('base', 'numExperimentTypes');

dataSeparation = 0.3;

% Get the x-axis values including a buffer on the first and last indices:
% Set to even itnerval for now
xTickVals = [0:2*numExperimentTypes];

xAxis = xTickVals(2:end);
  
% Subjects Plots

for p = 1:numExperimentTypes
    if(p == 1)
        expTypeColor = trainingMap1Color;
        i=1;j=2;
    end
    if(p == 2)
        expTypeColor = testingMap1Color;
        i=3;j=4;
    end
    errorbar([xAxis(i) xAxis(j)],...
        meanVals{1,p}, stdVals{1,p}, plotMarker, ...
        "MarkerFaceColor", expTypeColor, ...
        "Color", expTypeColor, "MarkerSize", markerSize, "LineWidth", 2);
    hold on;
end

%Plot Details
n=numExperimentTypes;
xlim([0.5 max(xTickVals)+2*dataSeparation]);
xticks(xTickVals);
tickLabels = [" ", repmat(["1", "Control"], 1, n)];
set(gca, 'xTick', xTickVals, 'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);
hold off;

end


