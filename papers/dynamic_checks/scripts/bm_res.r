#!/usr/bin/env Rscript

# You'll need some packages. Run the "bm_res_install.r" file in this dir.
suppressPackageStartupMessages(library(ggplot2))
suppressPackageStartupMessages(library(cowplot))
suppressPackageStartupMessages(library(scales))

results <- read.csv("bm_res.csv", header=TRUE, na.strings = "NA", strip.white = TRUE, comment.char = "#")

# Convert percentages to proportion [0..1]
results["LM."] = results["LM"]/100
results["EM."] = results["EM"]/100
results["LU."] = results["LU"]/100

# Convert % slowdowns to proportions [0..100] (1 = no change)
results["RT."] = (results["RT"]/100)+1
results["CT."] = (results["CT"]/100)+1
results["ES."] = (results["ES"]/100)+1

gm_mean = function(x){
  xprime <- na.omit(x)
  prod(xprime, na.rm=TRUE)^(1/length(xprime))
}

gmeans <- c()
gmeans$LM. = gm_mean(results$LM.)
gmeans$EM. = gm_mean(results$EM.)
gmeans$LU. = gm_mean(results$LU.)
gmeans$RT. = gm_mean(results$RT.)
gmeans$CT. = gm_mean(results$CT.)
gmeans$ES. = gm_mean(results$ES.)

#
# Plots
#

# Adds + / - (or no sign for zero) to percentages, to display 0.0 as "0%", 0.9 as "- 10%" and 1.1 as "+ 10%"
slowdown_formatter = function(datum) {
  datum_slowdown <- round(datum - 1, digits=3)
  positive <- !is.na(datum_slowdown) & datum_slowdown > 0
  paste0(ifelse(positive, "+ ", ""), percent(datum_slowdown))
}

plot_with_mean = function(name, data, key, mean, y_breaks, nudge=0, cat_label=NA) {
  if (is.na(cat_label)) {
    x_scale <- scale_x_discrete("", limits=rev(data$Name))
  } else {
    x_scale <- scale_x_discrete(cat_label, limits=rev(data$Name))
  }

  ggplot(dat=data, aes_(x=quote(Name), y=key)) +
    x_scale +
    scale_y_continuous(name,
                       labels=percent,
                       limits=c(min(y_breaks), max(y_breaks)),
                       breaks=y_breaks) +
    geom_col(aes(fill=Suite), na.rm = TRUE, position="dodge") +
    theme_gray() +
    theme(legend.position="none", axis.ticks.y = element_blank(), aspect.ratio = 1) +
    theme(panel.grid.major.y = element_blank()) +
    geom_hline(aes(), yintercept = mean, linetype="dotted") +
    geom_label(aes(Inf, mean, label=percent(mean), fill=NULL), vjust=1, hjust=ifelse(nudge<0,1,0), nudge_y=nudge, show.legend=FALSE) +
    coord_flip()
}

plot_slowdown_with_mean = function(name, data, key, mean, y_breaks, nudge=0, cat_label=NA) {
  if (is.na(cat_label)) {
    x_scale <- scale_x_discrete("", limits=rev(data$Name))
  } else {
    x_scale <- scale_x_discrete(cat_label, limits=rev(data$Name))
  }

  ggplot(dat=data, aes_(x=quote(Name), xend=quote(Name), yend=key, y=1.0)) +
    x_scale +
    scale_y_continuous(name,
                       labels=slowdown_formatter,
                       limits=c(min(y_breaks), max(y_breaks)),
                       breaks=y_breaks) +
    geom_segment(aes(color=Suite), na.rm = TRUE, arrow=arrow(length=unit(0.015, "npc"), angle=90)) +
    theme_gray() +
    theme(panel.grid.major.y = element_blank()) +
    theme(legend.position="none", axis.ticks.y = element_blank(), aspect.ratio = 1) +
    geom_hline(aes(), yintercept = mean, linetype="dotted") +
    geom_label(aes(x=Inf, y=mean, label=slowdown_formatter(mean), fill=NULL), vjust=1, hjust=ifelse(nudge<0,1,0), nudge_y=nudge, show.legend=FALSE) +
    coord_flip()
}

modifications = function() {
  pLM <- plot_with_mean("Lines Modified (%)",     results, ~LM., gmeans$LM., seq(0,0.4,0.1), 0.005,  cat_label="Benchmark")
  pEM <- plot_with_mean("Easy Modifications (%)", results, ~EM., gmeans$EM., seq(0,1.0,0.2), 0.01)
  pLU <- plot_with_mean("Lines Unchecked (%)",    results, ~LU., gmeans$LU., seq(0,0.25,0.05),  0.005)
  plot_grid(pLM, pEM, pLU + theme(legend.position="right"), nrow=1, align="hv", axis="tb", rel_widths=c(1,1,1.25))
}
ggsave("modifications.pdf", modifications(), "pdf", scale=2, width=6, height=2, units="in")

# modifications_pres = function() {
#   pLM <- plot_with_mean("Lines Modified (%)",     results, ~LM., gmeans$LM., seq(0,0.4,0.1), 0.005, cat_label="Benchmark")
#   pEM <- plot_with_mean("Easy Modifications (%)", results, ~EM., gmeans$EM., seq(0,1.0,0.2), 0.01)
#   pLU <- plot_with_mean("Lines Unchecked (%)",    results, ~LU., gmeans$LU., seq(0,0.25,0.05),  0.005, cat_label="Benchmark")
#   pTopRow <- plot_grid(pLM, pEM, ncol=2, align="hv", axis="tb")
#   plot_grid(pTopRow, pLU + theme(legend.position="right"), ncol=1, rel_heights=c(1.2,1))
# }
# ggsave("modifications_pres.pdf", modifications_pres(), "pdf", scale=1, width=6, height=6, units="in")



overheads = function () {
  pRT <- plot_slowdown_with_mean("Runtime Slowdown (±%)",       results, ~RT., gmeans$RT., seq(0.8,1.6,0.2), nudge=0.01, cat_label="Benchmark")
  pCT <- plot_slowdown_with_mean("Compile Time Slowdown (±%)",  results, ~CT., gmeans$CT., seq(1,2.0,0.25), nudge=0.01)
  pES <- plot_slowdown_with_mean("Executable Size Change (±%)", results, ~ES., gmeans$ES., seq(0.90,1.3,0.1), nudge=0.01)
  plot_grid(pRT, pCT, pES + theme(legend.position="right"), nrow=1, align="hv", axis="tb", rel_widths=c(1,1,1.25))
}
ggsave("overheads.pdf", overheads(), "pdf", scale=2, width=6, height=2, units="in")

# overheads_pres = function() {
#   pRT <- plot_slowdown_with_mean("Runtime Slowdown (±%)",       results, ~RT., gmeans$RT., seq(0.8,1.6,0.2), nudge=0.01, cat_label="Benchmark")
#   pCT <- plot_slowdown_with_mean("Compile Time Slowdown (±%)",  results, ~CT., gmeans$CT., seq(1,2.0,0.25), nudge=0.01)
#   pES <- plot_slowdown_with_mean("Executable Size Change (±%)", results, ~ES., gmeans$ES., seq(0.90,1.3,0.2), nudge=0.01, cat_label="Benchmark")
#   pTopRow <- plot_grid(pRT, pCT, ncol=2, align="hv", axis="tb")
#   plot_grid(pTopRow, pES + theme(legend.position="right"), ncol=1, rel_heights=c(1.2,1))
# }
# ggsave("overheads_pres.pdf", overheads_pres(), "pdf", scale=1, width=6, height=6, units="in")

#
# Tables
#

table_percent = function(datum) {
  ifelse(is.na(datum), datum, sprintf("%.1f", datum*100))
}

table_slowdown = function(datum) {
  datum_slowdown <- round(datum - 1, digits=3)
  positive <- datum_slowdown > 0
  negative <- datum_slowdown < 0

  prefix <- ifelse(positive, "+ ", ifelse(negative, "- ", ""))
  ifelse(is.na(datum), datum, paste0(prefix, table_percent(abs(datum_slowdown))))
}

split_at_anagram = function(datum) {
  paste0(ifelse(datum=="anagram", "\\addlinespace\n", ""), datum)
}


out_columns <- c("Name", "LM.", "EM.", "LU.", "RT.", "CT.", "ES.")
geo_mean_label = "\\midrule\n\\multicolumn{1}{r}{Geo. Mean:}"
geo_mean_row <- data.frame(geo_mean_label, gmeans$LM., gmeans$EM., gmeans$LU., gmeans$RT., gmeans$CT., gmeans$ES.)
names(geo_mean_row) <- out_columns
results_out <- rbind(results[out_columns], geo_mean_row)

results_out[c("LM.", "EM.", "LU.")] <- lapply(results_out[c("LM.", "EM.", "LU.")], table_percent)
results_out[c("RT.", "CT.", "ES.")] <- lapply(results_out[c("RT.", "CT.", "ES.")], table_slowdown)
results_out["Name"] = lapply(results_out["Name"], split_at_anagram)

write.table(results_out[out_columns], "bm_results_tab.tex", quote=FALSE, sep=" & ", eol=" \\\\\n", na="--", row.names = FALSE, col.names=FALSE)

stat_commands = function(kind, mean, max=NA, min=NA) {
  stat_percent = function(datum) {
    ifelse(is.na(datum), "??", sprintf("%.1f", datum*100))
  }

  mean_cmd <- sprintf("\\newcommand{\\Result%sMean}{%s\\%%\\xspace}", kind, stat_percent(mean))
  max_cmd <- sprintf("\\newcommand{\\Result%sMax}{%s\\%%\\xspace}", kind, stat_percent(max))
  min_cmd <- sprintf("\\newcommand{\\Result%sMin}{%s\\%%\\xspace}", kind, stat_percent(min))
  paste0("\n", mean_cmd, "\n", max_cmd, "\n", min_cmd, "\n")
}

sink("bm_results_macros.tex")
cat("% Auto Generated by bm_res.r\n% Do Not Modify")
cat(stat_commands("LinesModified", gmeans$LM., max(results$LM.,
na.rm=TRUE), min(results$LM., na.rm=TRUE)))
cat(stat_commands("EasyModifications", gmeans$EM., max(results$EM.,
na.rm=TRUE), min(results$EM., na.rm=TRUE)))
cat(stat_commands("LinesUnchecked", gmeans$LU., max(results$LU.,
na.rm=TRUE), min(results$LU., na.rm=TRUE)))
cat(stat_commands("RunTime", gmeans$RT.-1, max(results$RT.,
na.rm=TRUE)-1, min(results$RT., na.rm=TRUE)-1))
cat(stat_commands("CompileTime", gmeans$CT.-1, max(results$CT.,
na.rm=TRUE)-1, min(results$CT., na.rm=TRUE)-1))
cat(stat_commands("ExecutableSize", gmeans$ES.-1, max(results$ES.,
na.rm=TRUE)-1, min(results$ES., na.rm=TRUE)-1))
sink()
