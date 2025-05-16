#include "algorithms/genetic_algorithm.hpp"

#include <algorithm>
#include <memory>

#include "algorithms/population_generator.hpp"
#include "algorithms/cross_over_operator.hpp"

static constexpr int kMaxStagnationCount = 200;
static constexpr int kMaxPopulationSize = 1000000;
static constexpr int kMaxIndividualSize = 1000000;

GeneticAlgorithm::GeneticAlgorithm()
    : population_size_(0),
      individual_size_(0),
      selection_rate_(-1.),
      real_time_callback_(false) {}

static void SortPopulation(Population& unsorted_population) {
  auto& population = unsorted_population.GetPopulation();
  std::sort(population.begin(), population.end(),
            [](const std::shared_ptr<Individual>& left,
               const std::shared_ptr<Individual>& right) {
              return left->GetTotalDistance() < right->GetTotalDistance();
            });
}

void GeneticAlgorithm::SetUpPopulation() {
  population_ = population_generator::GenerateNewPopulation(population_size_,
                                                            individual_size_);
  auto& population = population_.GetPopulation();

  for (std::shared_ptr<Individual>& individual : population) {
    individual_manager_.ResetDistance(*individual);
  }

  SortPopulation(population_);
}

void GeneticAlgorithm::SetUpBestIndividual() {
  best_individual_ = *population_.GetPopulation()[0];

  for (const auto& individual : population_.GetPopulation()) {
    if (individual->GetTotalDistance() < best_individual_.GetTotalDistance()) {
      best_individual_ = *individual;
    }
  }
}

void GeneticAlgorithm::SetUpIndividualManager() {
  individual_manager_.SetMutationRate(0.2);
  individual_manager_.SetMutationPolicy(IndividualManager::kReverse);
}

bool GeneticAlgorithm::Cycle() {
  const std::size_t to_keep =
      std::max(static_cast<int>(population_size_ * selection_rate_), 2);

  int individual_created = 0;

  while (individual_created + to_keep < population_size_) {
    for (int i = 0; i < to_keep; i++) {
      for (int j = 1; j < to_keep; j++) {
        if (individual_created + to_keep >= population_size_) {
          break;
        }

        auto& individual_to_eliminate =
            population_.GetPopulation()[individual_created + to_keep];
        individual_to_eliminate = std::make_shared<Individual>(
            cross_over::CrossOver(*population_.GetPopulation()[i],
                                  *population_.GetPopulation()[j]));
        individual_manager_.MutateIndividual(*individual_to_eliminate);
        individual_manager_.ResetDistance(*individual_to_eliminate);
        individual_created++;
      }

      if (individual_created + to_keep >= population_size_) {
        break;
      }
    }
  }

  SortPopulation(population_);

  if (population_.GetPopulation()[0]->GetTotalDistance() <
      best_individual_.GetTotalDistance()) {
    best_individual_ = *population_.GetPopulation()[0];
  }
  return true;
}

bool GeneticAlgorithm::Process() {
  if (individual_size_ <= 0) {
    return false;
  }

  if (population_size_ <= 0) {
    return false;
  }

  if (individual_size_ >= kMaxIndividualSize) {
    return false;
  }

  if (population_size_ >= kMaxPopulationSize) {
    return false;
  }

  if (selection_rate_ <= 0) {
    return false;
  }

  if (selection_rate_ >= 1) {
    return false;
  }

  if (individual_manager_.GetMap().size() != individual_size_) {
    return false;
  }

  SetUpPopulation();
  SetUpBestIndividual();
  SetUpIndividualManager();

  int stagnation_count_guard = 0;

  int generation_count = 0;
  while (stagnation_count_guard <= kMaxStagnationCount) {
    const double current_best = best_individual_.GetTotalDistance();
    Cycle();
    const double new_best = best_individual_.GetTotalDistance();

    if (new_best < current_best) {
      stagnation_count_guard = 0;
      generation_count++;
      if (real_time_callback_) {
        progress_callback_(best_individual_.GetCoordinateList(), new_best,
                           generation_count);
      }
      continue;
    }

    stagnation_count_guard++;
    generation_count++;
  }
  return true;
}
