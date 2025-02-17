#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "algo.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <random>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ApplyModel();
}

MainWindow::~MainWindow() {
    delete ui;
    std::random_device rd;
    new_random_gen = std::mt19937(rd());

}

void MainWindow::ApplyModel() {
    auto preserve_iter = deque_model_.iterator;

    ui->list_widget->clear();

    if (deque_model_.items.empty()) {
        ui->txt_size->setText("0");
        ui->list_widget->addItem("end");
        ApplyIterator();
        return;
    }

    for (size_t i = 0; i < deque_model_.items.size(); ++i) {
        ui->list_widget->addItem(QString::fromStdString(std::to_string(i) + ": " + deque_model_.items[i]));
    }

    ui->list_widget->addItem("end");

    ui->txt_size->setText(QString::number(deque_model_.items.size()));
    deque_model_.iterator = preserve_iter;

    ApplyIterator();
}

void MainWindow::ApplyIterator() {
    auto distance = std::distance(deque_model_.items.begin(), deque_model_.iterator);
    ui->list_widget->setCurrentRow(distance >= 0 ? distance : -1);

    bool at_end = deque_model_.iterator == deque_model_.items.end();
    bool at_begin = deque_model_.iterator == deque_model_.items.begin();
    bool empty = deque_model_.items.empty();
    ui->pb_edit->setEnabled(!at_end);
    ui->pb_erase->setEnabled(!at_end);
    ui->pb_increment->setEnabled(!at_end);
    ui->pb_decrement->setEnabled(!at_begin);
    ui->btn_pop_back->setEnabled(!empty);
    ui->btn_pop_front->setEnabled(!empty);

    if (at_end) {
        ui->txt_elem_content->clear();
    } else {
        ui->txt_elem_content->setText(QString::fromStdString(*deque_model_.iterator));
    }
}


void MainWindow::SetRandomGen(const std::mt19937& random_gen){
    new_random_gen = random_gen;
}

void MainWindow::on_pb_tea_clicked()
{
    deque_model_.items.clear();
    ApplyModel();
    static std::deque<std::string> tea {
        "Чай Лунцзин",
        "Эрл Грей",
        "Сенча",
        "Пуэр",
        "Дарджилинг",
        "Ассам",
        "Матча",
        "Ганпаудер",
        "Оолонг",
        "Лапсанг Сушонг"
    };
    for (const auto& i : tea){
        deque_model_.items.push_back(i);
    }
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();

}

void MainWindow::on_pb_cakes_clicked()
{
    deque_model_.items.clear();
    ApplyModel();
    static std::deque<std::string> cakes {
        "Красный бархат",
        "Наполеон",
        "Медовик",
        "Тирамису",
        "Прага",
        "Чизкейк",
        "Захер",
        "Эстерхази",
        "Морковный торт",
        "Чёрный лес",
    };

    for (const auto& i : cakes){
        deque_model_.items.push_back(i);
    }
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();

}


void MainWindow::on_btn_pop_back_clicked() {
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_back();
        deque_model_.iterator = deque_model_.items.empty() ? deque_model_.items.end() : deque_model_.items.end() - 1;
        deque_model_.iterator = deque_model_.items.begin();
        ApplyModel();
    }
}

void MainWindow::on_btn_pop_front_clicked()
{
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_front();
        deque_model_.iterator = deque_model_.items.empty() ? deque_model_.items.end() : deque_model_.items.end() - 1;
        deque_model_.iterator = deque_model_.items.begin();
        ApplyModel();
    }
}




void MainWindow::on_pb_clear_clicked()
{
    deque_model_.items.clear();
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}


void MainWindow::on_pb_push_back_clicked()
{
    deque_model_.items.push_back(ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}


void MainWindow::on_pb_push_front_clicked()
{
    deque_model_.items.push_front(ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}




void MainWindow::on_pb_erase_clicked() {
    if (deque_model_.iterator != deque_model_.items.end()) {
        deque_model_.iterator = deque_model_.items.erase(deque_model_.iterator);
        deque_model_.iterator = deque_model_.items.begin();
        ApplyModel();
    }
}


void MainWindow::on_pb_insert_clicked() {
    deque_model_.iterator = deque_model_.items.insert(deque_model_.iterator, ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}


void MainWindow::on_pb_decrement_clicked()
{
    if (deque_model_.iterator != deque_model_.items.begin()) {
        --deque_model_.iterator;
        ApplyIterator();
    }
}


void MainWindow::on_pb_increment_clicked()
{
    if (deque_model_.iterator != deque_model_.items.end()) {
        ++deque_model_.iterator;
        ApplyIterator();
    }
}


void MainWindow::on_pb_begin_clicked() {
    deque_model_.iterator = deque_model_.items.begin();
    ApplyIterator();
}

void MainWindow::on_pb_end_clicked() {
    deque_model_.iterator = deque_model_.items.end();
    ApplyIterator();
}


void MainWindow::on_pb_edit_clicked()
{
    if(deque_model_.iterator != deque_model_.items.end()){
        *deque_model_.iterator = ui->txt_elem_content->text().toStdString();
    }
    ApplyModel();
}


void MainWindow::on_list_widget_currentRowChanged(int currentRow) {
    currentRow = std::clamp(currentRow, 0, (int)deque_model_.items.size());
    if (currentRow < 0 || currentRow >= deque_model_.items.size()) {
        deque_model_.iterator = deque_model_.items.end();
    } else {
        deque_model_.iterator = deque_model_.items.begin() + currentRow;
    }
    ApplyIterator();
}



void MainWindow::on_btn_resize_clicked()
{
    int s = ui->txt_size->text().toInt();
    if(s < 1000){
        deque_model_.items.resize(s);
        deque_model_.iterator = deque_model_.items.begin();
        ApplyModel();
    }

}


void MainWindow::on_btn_count_clicked()
{
    std::string s = ui->le_count->text().toStdString();
    int count = std::count(deque_model_.items.begin(), deque_model_.items.end(), s);
    ui->lbl_count->setText(QString::number(count));
    ApplyModel();
}


void MainWindow::on_btn_find_clicked()
{
    std::string s = ui->txt_elem_content->text().toStdString();
    auto it = std::find(deque_model_.items.begin(), deque_model_.items.end(), s);
    deque_model_.iterator = it;
    ApplyIterator();
}


void MainWindow::on_pb_min_clicked()
{
    auto it = std::min_element(deque_model_.items.begin(), deque_model_.items.end());
    deque_model_.iterator = it;
    ApplyIterator();
}


void MainWindow::on_pb_max_clicked()
{
    auto it = std::max_element(deque_model_.items.begin(), deque_model_.items.end());
    deque_model_.iterator = it;
    ApplyIterator();
}


void MainWindow::on_btn_merge_sort_clicked()
{
    deque_model_.items = MergeSort(deque_model_.items, std::less());

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_merge_sOrT_clicked()
{

    deque_model_.items = MergeSort(deque_model_.items, [](const std::string& a, const std::string& b) {
        return std::lexicographical_compare(
            a.begin(), a.end(), b.begin(), b.end(),
            [](char c1, char c2) { return std::tolower(c1) < std::tolower(c2); }
            );
    });

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();


}


void MainWindow::on_brn_unique_clicked()
{

    if(std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())){

        auto last = std::unique(deque_model_.items.begin(), deque_model_.items.end());
        deque_model_.items.erase(last, deque_model_.items.end());
        deque_model_.iterator = deque_model_.items.begin();

    }

    ApplyModel();

}


void MainWindow::on_btn_shuffle_clicked()
{
    std::shuffle(deque_model_.items.begin(), deque_model_.items.end(), new_random_gen);
    ApplyModel();
}


void MainWindow::on_btn_reverse_clicked()
{
    std::reverse(deque_model_.items.begin(), deque_model_.items.end());
    ApplyModel();
}







void MainWindow::on_brn_lower_bound_clicked()
{


    if (std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        std::string str = ui->txt_elem_content->text().toStdString();
        auto lower = std::lower_bound(deque_model_.items.begin(), deque_model_.items.end(), str);
        deque_model_.iterator = lower;
        ApplyModel();
    }



}


void MainWindow::on_btn_upper_bound_clicked()
{

    if (std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        std::string str = ui->txt_elem_content->text().toStdString();
        auto upper = std::upper_bound(deque_model_.items.begin(), deque_model_.items.end(), str);
        deque_model_.iterator = upper;
        ApplyModel();
    }



}

