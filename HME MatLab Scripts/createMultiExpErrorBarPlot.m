%   Creates an errorbar plot with trial data
%   Also assumes subjects are sorted columnwise
%   in the SubjectData cell
% Author: Jasmin E. Palmer

function [h1, h2, h3] = createMultiExpErrorBarPlot(meanVals, stdVals,...
    plotTitle, xAxisLabel, yAxisLabel)

% Pull variables from workspace
trainingMap5Color = evalin('base','trainingMap5Color');
testingMap5Color = evalin('base','testingMap5Color');
jitterVal = evalin('base','jitterVal');
numMappings = evalin('base','numMappings');
plotMarker = evalin('base','plotMarker');
markerSize = evalin('base', 'markerSize');
numExperimentTypes = evalin('base', 'numExperimentTypes');
minY =  evalin('base', 'minY');
maxY =  evalin('base', 'maxY');
alphaVal = evalin('base', 'alphaVal');
yVal = evalin('base', 'yVal');
dataSeparation = 0.3;
 minX = 0.5;

% Get the x-axis values including a buffer on the first and last indices:
% Set to even itnerval for now
xTickVals = [0:3*numMappings];

xAxis = xTickVals(2:end);

% Shade only the backgrounds of training data
xT = 0.5*(xAxis(6)+xAxis(7)); % X-vlaue of test-train border
% x = [minX      xT  xT  minX];
% y = [minY minY  maxY  maxY];
% patch(x,y,'r','alpha');
v = [minX minY; xT minY; xT maxY; minX maxY];
f = [1 2 3 4];
shadeColor = [0.5 0.5 0.5];

patch('Faces',f,'Vertices',v,'FaceColor',num2str(shadeColor),...
    'EdgeColor',num2str(shadeColor),...
    'EdgeAlpha',alphaVal,'FaceAlpha',alphaVal); hold on;


% Section labels:
textYCoord = maxY-yVal;
text(0.5*(xAxis(3)+xAxis(4)), textYCoord,...
    "Training", "HorizontalAlignment","center", "FontSize", 20)
text(xAxis(8), textYCoord,...
    "Testing", "HorizontalAlignment","center", "FontSize", 20)

% Subjects Plots
for p = 1:numExperimentTypes
    if(p == 1)
        expTypeColor = trainingMap5Color;
        % i=1;j=2;k=3;

        trainingMeans = reshape(meanVals{1,p},[2,3]);
        trainingStd = reshape(stdVals{1,p},[2,3]);

        h1 = errorbar([xAxis(1) xAxis(2) xAxis(3)]+jitterVal,...
            trainingMeans(1,:), trainingStd(1,:), plotMarker, ...
            "MarkerFaceColor", expTypeColor, ...
            "Color", expTypeColor, "MarkerSize", markerSize, "LineWidth", 2);
        hold on;

        h2 = errorbar([xAxis(4) xAxis(5) xAxis(6)]+jitterVal,...
            trainingMeans(2,:), trainingStd(2,:), plotMarker, ...
            "MarkerFaceColor", expTypeColor, ...
            "Color", expTypeColor, "MarkerSize", markerSize, "LineWidth", 2);
        hold on;
    end
    if(p == 2)
        expTypeColor = trainingMap5Color;
        i=4;j=5;k=6;

        h3 = errorbar([xAxis(7) xAxis(8) xAxis(9)]+jitterVal,...
            meanVals{1,p}, stdVals{1,p}, plotMarker, ...
            "MarkerFaceColor", expTypeColor, ...
            "Color", expTypeColor, "MarkerSize", markerSize, "LineWidth", 2);
        hold on;
    end
end

% Separation lines:
% Between training 1 and training 2
%xline(0.5*(xAxis(3)+xAxis(4)));
% Between training 2 and testing
% xline(0.5*(xAxis(6)+xAxis(7)));


% Plot Details
n=numExperimentTypes+1; % for the duplicate p = 1
xlim([minX max(xTickVals)+2*dataSeparation]);
xticks(xTickVals);
tickLabels = [" ", repmat(["Dual Tactor", "Single Tactor", "Control"], 1, n)];
set(gca, 'xTick', xTickVals, 'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);

hold off;

end


