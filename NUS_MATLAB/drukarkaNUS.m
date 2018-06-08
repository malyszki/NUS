s = serial('COM3')
s.BaudRate = 9600
fopen(s)
% fprintf(s,'X:100 Y:100 Z:100')

% if ~exist('s','var')
%     s = serial('COM3')
%     s.BaudRate = 9600
%     fopen(s)
% end
% 
% % krokiX = [0 100 -100 0 100 -100 0];
% % krokiY = [0 0 100 -100 0 100 -100];
% % krokiZ = [0 0 0 100 -100 0 100];
% % 
% % for i = 1:length(krokiX)
% %     fprintf(s,'X:%d Y:%d Z:%d\n', [krokiX(i) krokiY(i) krokiZ(i)])
% %     pause(0.1)
% % end

t = 0:0.1:10;

trajX = round(sin(t)*2000); 
trajY = round(sin(t + (1/3*pi))*2000); 
trajZ = round(sin(t + (2/3*pi))*2000);

krokiX = trajX(2:end)-trajX(1:end-1);
krokiY = trajY(2:end)-trajY(1:end-1);
krokiZ = trajZ(2:end)-trajZ(1:end-1);

for i = 1:length(krokiX)
    fprintf(s,'X:%d Y:%d Z:%d\n', [krokiX(i) krokiY(i) krokiZ(i)])
    while(s.BytesAvailable == 0)
        ;
    end
    fscanf(s)
end
