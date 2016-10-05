% 提取 win10 锁屏界面的壁纸
clc,clear,close all;
%% 提取横屏壁纸
path=fullfile(tempdir(),...
    '..\Packages\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\LocalState\Assets');
fs=getFiles(path);
ind=logical(cellfun(@(x)imFilter(x),fs));
fs=fs(ind);
n=length(fs);
nfs=fs;
%% 保存到 此电脑>图片>保存的图片 文件夹
outpath=fullfile(getenv('USERPROFILE'),'.\Pictures\Saved Pictures');
if ~exist(outpath,'dir')
    mkdir(outpath);
end
% 获取当前文件夹下已经添加的壁纸
fs=getFiles(outpath);
ind=cellfun(@(x)~isempty(regexp(x,'\d+\.jpg','once')),fs);
ns=cellfun(@(x)str2double(regexp(x,'\d+(?=\.jpg)','match')),fs(ind));
if isempty(ns)
    NF=0;
else
    NF=max(ns);
end
% 复制新提取的壁纸
for i=1:n
    copyfile(nfs{i},fullfile(outpath,sprintf('%03d.jpg',i+NF)),'f');
end
% 壁纸去重
md5unique(getFiles(outpath));
%% 打开目标文件夹
winopen(outpath);

