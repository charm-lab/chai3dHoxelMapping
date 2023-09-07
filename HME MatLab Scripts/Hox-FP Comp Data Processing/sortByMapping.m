%** Sorts subjects form expriment by their mapping condition
%   Also assumes subjects are sorted columnwise:
%   [visibleCube | invisibleCube]
%   in the SubjectData cell

function sortedVariable = sortByMapping(parameter, mapping)

%Pull data from workspace
numSubjects = evalin('base','numSubjects');

% sortedVariable = parameter(mapping);

sortedVariable(:,1) = parameter(mapping(1,:),1);


end