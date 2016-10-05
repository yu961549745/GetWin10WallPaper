% ��ȡ win10 ��������ı�ֽ
clc,clear,close all;
%% ��ȡ������ֽ
path=fullfile(tempdir(),...
    '..\Packages\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\LocalState\Assets');
fs=getFiles(path);
ind=logical(cellfun(@(x)imFilter(x),fs));
fs=fs(ind);
n=length(fs);
nfs=fs;
%% ���浽 �˵���>ͼƬ>�����ͼƬ �ļ���
outpath=fullfile(getenv('USERPROFILE'),'.\Pictures\Saved Pictures');
if ~exist(outpath,'dir')
    mkdir(outpath);
end
% ��ȡ��ǰ�ļ������Ѿ���ӵı�ֽ
fs=getFiles(outpath);
ind=cellfun(@(x)~isempty(regexp(x,'\d+\.jpg','once')),fs);
ns=cellfun(@(x)str2double(regexp(x,'\d+(?=\.jpg)','match')),fs(ind));
if isempty(ns)
    NF=0;
else
    NF=max(ns);
end
% ��������ȡ�ı�ֽ
for i=1:n
    copyfile(nfs{i},fullfile(outpath,sprintf('%03d.jpg',i+NF)),'f');
end
% ��ֽȥ��
md5unique(getFiles(outpath));
%% ��Ŀ���ļ���
winopen(outpath);

